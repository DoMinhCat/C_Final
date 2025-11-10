/*
Date of creation : 08/11/2025
Description : Code of INSERT statement
Group 2 ESGI 2A3
*/

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>
#include <string.h>
#include <math.h>

#include "db.h"
#include "helper_db.h"
#include "../ui/parser.h"
#include "../hash/hash.h"
#include "../init/init.h"

void free_insert_before_exit(int** int_list_to_insert, char*** str_list_to_insert, double** double_list_to_insert, int str_item_count){
    if(*int_list_to_insert){
        free(*int_list_to_insert);
        *int_list_to_insert = NULL;
    }
    if(*str_list_to_insert){
        for(int i=0; i<str_item_count; i++){
            free((*str_list_to_insert)[i]);
            (*str_list_to_insert)[i] = NULL;
        }
        free(*str_list_to_insert);
        *str_list_to_insert = NULL;
    }
    if(*double_list_to_insert){
        free(*double_list_to_insert);
        *double_list_to_insert = NULL;
    }
}

void insert(Query* query){
    char** col_list = query->params.insert_params.col_list;
    int col_count = query->params.insert_params.col_count;
    char** data_list = query->params.insert_params.data_list;

    Table* table = NULL;
    //ColType* type_list = NULL; //(ColType*)malloc(sizeof(ColType) * col_count); // IMPORTANT: free at early return
    Col* current_col = NULL;
    Row* new_row = NULL;
    HashTable* first_hash_tab = NULL;
    HashTable* hash_tab_of_col = NULL;
    bool col_exist;
    bool int_pk_provided = false;

    // list storing validated field for insert later
    int* int_list_to_insert = NULL;
    char** str_list_to_insert = NULL;
    double* double_list_to_insert = NULL;
    int int_item_count = 0;
    int str_item_count = 0;
    int double_item_count = 0;

    char* endptr;
    long long parsed_val;
    double double_val;
    int i;
    int int_pk_val;
    int safe_val;

    // check table exist
    table = get_table_by_name(query->params.insert_params.table_name);
    if(!table) {
        fprintf(stderr, "Execution error: '%s' table not found.\n", query->params.insert_params.table_name);
        return;
    }
    first_hash_tab = table->first_hash_table;

    // checks for cols to insert and init list for later insert
    for(i=0; i<col_count; i++){
        
        col_exist = false;

        // Loop through cols of table
        for(current_col = table->first_col; current_col!=NULL; current_col = current_col->next_col) {
            // search for col
            if (strcmp(current_col->name, col_list[i]) == 0) {
                col_exist = true;
                errno = 0;

                // found col, do checks based on data type of col
                switch (current_col->type)
                {
                case INT:
                    errno = 0;
                    parsed_val = strtoll(data_list[i], &endptr, 10);
                    // debug
                    printf("DEBUG: parsed_val = %lld, errno = %d, ERANGE = %d\n", parsed_val, errno, ERANGE);
                    printf("DEBUG: endptr check: endptr=%p, data_list[i]=%p, *endptr='%c' (0x%02x)\n", (void*)endptr, (void*)data_list[i], *endptr, (unsigned char)*endptr);

                    // check conversion error
                    if (endptr == data_list[i] || *endptr != '\0') {
                        fprintf(stderr, "Execution error: invalid value '%s' for '%s' column  type INT.\n",
                                data_list[i], col_list[i]);
                        free_insert_before_exit(&int_list_to_insert, &str_list_to_insert, &double_list_to_insert, str_item_count);
                        return;
                    }

                    // check overflow for type int
                    if (errno == ERANGE || parsed_val > INT_MAX || parsed_val < INT_MIN) {
                        fprintf(stderr, "Execution error: incompatible size of value '%s' for type INT.\n", data_list[i]);
                        free_insert_before_exit(&int_list_to_insert, &str_list_to_insert, &double_list_to_insert, str_item_count);
                        return;
                    }

                    safe_val = (int)parsed_val;

                    // FK check referential integrity
                    if (current_col->constraint == FK) {
                        if(safe_val <= 0){
                            fprintf(stderr, "Execution error: values with FOREIGN KEY constraint must be 1 or larger.\n");
                            free_insert_before_exit(&int_list_to_insert, &str_list_to_insert, &double_list_to_insert, str_item_count);
                            return;
                        }
                        if(!refer_val_exists(NULL, safe_val, current_col->refer_table, current_col->refer_col)){
                            free_insert_before_exit(&int_list_to_insert, &str_list_to_insert, &double_list_to_insert, str_item_count);
                            return;
                        }
                    }

                    // Uniqueness check
                    if (current_col->constraint == PK || current_col->constraint == UNIQUE) {
                        // flag for auto increment later
                        if(current_col->constraint == PK){
                            int_pk_provided = true;
                            int_pk_val = safe_val;
                        }
                        hash_tab_of_col = get_ht_by_col_name(first_hash_tab, current_col->name);
                        if (!is_unique_hash(NULL, safe_val, hash_tab_of_col)) {
                            free_insert_before_exit(&int_list_to_insert, &str_list_to_insert, &double_list_to_insert, str_item_count);
                            return;
                        }
                    }

                    // Store validated value in a list to insert later
                    int_list_to_insert = realloc(int_list_to_insert, sizeof(int) * (int_item_count + 1));
                    assert(int_list_to_insert != NULL);
                    int_list_to_insert[int_item_count++] = safe_val;
                    break;
                case DOUBLE:
                    errno = 0;
                    double_val = strtod(data_list[i], &endptr);

                    // check conversion error
                    if (endptr == data_list[i] || *endptr != '\0' || isinf(double_val) || isnan(double_val) || errno == ERANGE) {
                        fprintf(stderr, "Execution error: invalid value '%s' for '%s' column  type DOUBLE.\n", data_list[i], col_list[i]);
                        free_insert_before_exit(&int_list_to_insert, &str_list_to_insert, &double_list_to_insert, str_item_count);
                        return;
                    }

                    // no check for unique/pk/fk needed, double type not allowed to have unique/pk/fk constraint

                    // expand temp list and store validated value
                    double_list_to_insert = (double*)realloc(double_list_to_insert, sizeof(double) * (double_item_count+1));
                    assert(double_list_to_insert!=NULL);
                    double_list_to_insert[double_item_count] = (double)double_val;

                    double_item_count++;
                    break;
                case STRING:
                    // no need to convert type, data_list is stored as string

                    // empty value not allowed
                    if(data_list[i]){
                        if(strcmp(data_list[i], "") == 0){
                            fprintf(stderr, "Execution error: empty string not allowed.\n");
                            free_insert_before_exit(&int_list_to_insert, &str_list_to_insert, &double_list_to_insert, str_item_count);
                            return;
                        }
                    }

                    // check max_len 256
                    if(strlen(data_list[i]) > MAX_STR_LEN){
                        fprintf(stderr, "Execution error: 256 characters maximum allowed for STRING values.\n");
                        free_insert_before_exit(&int_list_to_insert, &str_list_to_insert, &double_list_to_insert, str_item_count);
                        return;
                    }

                    //TODO check fk: referential integrity
                    if(current_col->constraint == FK){
                        if(!refer_val_exists(data_list[i], 0, current_col->refer_table, current_col->refer_col)){
                            free_insert_before_exit(&int_list_to_insert, &str_list_to_insert, &double_list_to_insert, str_item_count);
                            return;
                        }
                    }
                  
                    // check UNIQUE constraint and pk uniqueness
                    if(current_col->constraint == PK || current_col->constraint == UNIQUE){ 
                        hash_tab_of_col = get_ht_by_col_name(first_hash_tab, current_col->name);
                        if(!is_unique_hash(data_list[i], 0, hash_tab_of_col)){
                            free_insert_before_exit(&int_list_to_insert, &str_list_to_insert, &double_list_to_insert, str_item_count);
                            return;
                        }
                    }

                    // expand temp list and store validated value
                    str_list_to_insert = (char**)realloc(str_list_to_insert, sizeof(char*) * (str_item_count+1));
                    assert(str_list_to_insert!=NULL);
                    str_list_to_insert[str_item_count] = strdup(data_list[i]);
                    assert(str_list_to_insert[str_item_count]!=NULL);
                    
                    str_item_count++;
                    break;
                default:
                    fprintf(stderr, "Execution error: unknown type for '%s' column .\n", col_list[i]);
                    free_insert_before_exit(&int_list_to_insert, &str_list_to_insert, &double_list_to_insert, str_item_count);
                    return;
                    break;
                }
                break;
            }
        }

        if(!col_exist){
            fprintf(stderr, "'%s' column  not found.\n", col_list[i]);
            free_insert_before_exit(&int_list_to_insert, &str_list_to_insert, &double_list_to_insert, str_item_count);
            return;
        }
    }


    // start inserting after all checks passed
    int str_col_count = 0;
    int double_col_count = 0;
    int int_col_count = 0;

    new_row = init_row();
//TODO malloc 3 lists
    // get size to malloc for each type
    for(current_col=table->first_col; current_col!=NULL; current_col=current_col->next_col){
        if(current_col->type == INT) int_col_count++;
        else if(current_col->type == STRING) str_col_count++;
        else if(current_col->type == DOUBLE) double_col_count++;
    }
    new_row->double_list = (double*)calloc(double_col_count, sizeof(double));

    //insert str list
    assert((new_row->str_list = (char**)malloc(sizeof(char*) * str_item_count)) != NULL); //wrong
    

    // handle auto incrementation of int pk
    // if(int_pk_provided){
    //     table->next_id = int_pk_val+1;
    // }else{
    //     table->next_id++;
    // }

    //TODO : set id, check id, auto increment (see ideas in README)
    // TODO : hash id then add to hash table of this table
    // TODO : cast data fields to its correct type and add to row
    // TODO : also hash unique col and add to corresponding hash table
    free_insert_before_exit(&int_list_to_insert, &str_list_to_insert, &double_list_to_insert, str_item_count);
    fprintf(stdout, "Executed: a new row with %d %s inserted into '%s' table.\n", col_count, col_count>1?"values":"value", table->name);
}