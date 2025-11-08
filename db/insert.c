/*
Date of creation : 08/11/2025
Description : Code of INSERT statement
Group 2 ESGI 2A3
*/

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>

#include "db.h"
#include "helper_db.h"
#include "../ui/parser.h"

void free_insert_before_exit(int** int_list_to_insert, char*** str_list_to_insert, double** double_list_to_insert, int str_item_count){
    free(*int_list_to_insert);
    *int_list_to_insert = NULL;
    for(int i=0; i<str_item_count; i++){
        free((*str_list_to_insert)[i]);
        (*str_list_to_insert)[i] = NULL;
    }
    free(*str_list_to_insert);
    *str_list_to_insert = NULL;
    free(*double_list_to_insert);
    *double_list_to_insert = NULL;
}

void insert(Query* query){
    char** col_list = query->params.insert_params.col_list;
    int col_count = query->params.insert_params.col_count;
    char** data_list = query->params.insert_params.data_list;

    Table* table = NULL;
    //ColType* type_list = NULL; //(ColType*)malloc(sizeof(ColType) * col_count); // IMPORTANT: free at early return
    Col* current_col = table->first_col;
    bool col_exist;

    // list storing validated field for insert later
    int* int_list_to_insert = NULL;
    char** str_list_to_insert = NULL;
    double* double_list_to_insert = NULL;
    int int_item_count = 0;
    int str_item_count = 0;
    int double_item_count = 0;

    int i,j;

    // check table exist
    table = get_table_by_name(query->params.insert_params.table_name);
    if(!table) {
        fprintf(stderr, "Execution error: table '%s' not found.\n", query->params.insert_params.table_name);
        return;
    }

    // checks for cols to insert
    for(i=0; i<col_count; i++){

        // Loop through cols of table
        for(j=0; j<table->col_count; j++) {
            col_exist = false;

            // search for col
            if (strcmp(current_col->name, col_list[i]) == 0) {
                col_exist = true;
                
                // found col, check type col same with data type
                switch (current_col->type)
                {
                case INT:
                    char* endptr;
                    long int_val;
                    errno = 0;

                    int_val = strtol(data_list[i], &endptr, 10);

                    // check conversion error
                    if (endptr == data_list[i] || *endptr != '\0') {
                        fprintf(stderr, "Execution error: invalid value '%s' for column '%s' type INT.\n", data_list[i], col_list[i]);
                        free_insert_before_exit(&int_list_to_insert, &str_list_to_insert, &double_list_to_insert, str_item_count);
                        return;
                    }

                    // check overflow for type int
                    if (data_list[i] > INT_MAX || data_list[i] < INT_MIN || errno == ERANGE) {
                        printf("Execution error: incompatible size of value '%s' for type INT.\n");
                        free_insert_before_exit(&int_list_to_insert, &str_list_to_insert, &double_list_to_insert, str_item_count);
                        return; 
                    }

                    // check uniqueness constraint UNIQUE
                    if(current_col->constraint == UNIQUE){
                        if(!is_unique_int(table, col_list[i], data_list[i])) {
                            free_insert_before_exit(&int_list_to_insert, &str_list_to_insert, &double_list_to_insert, str_item_count);
                            return;
                        }
                    }

                    // expand temp list and store validated value
                    int_list_to_insert = (int*)realloc(int_list_to_insert, sizeof(int) * (int_item_count+1));
                    assert(int_list_to_insert!=NULL);
                    int_list_to_insert[int_item_count] = (int)int_val;

                    int_item_count++;
                    break;
                case DOUBLE:
                    char* endptr;
                    double double_val;
                    errno = 0;

                    double_val = strtol(data_list[i], &endptr, 10);

                    // check conversion error
                    if (endptr == data_list[i] || *endptr != '\0' || isinf(data_list[i]) || isnan(data_list[i]) || errno == ERANGE) {
                        fprintf(stderr, "Execution error: invalid value '%s' for column '%s' type DOUBLE.\n", data_list[i], col_list[i]);
                        free_insert_before_exit(&int_list_to_insert, &str_list_to_insert, &double_list_to_insert, str_item_count);
                        return;
                    }

                    // no check for unique needed, double type not allowed to have unique or pk constraint

                    // expand temp list and store validated value
                    double_list_to_insert = (double*)realloc(double_list_to_insert, sizeof(double) * (double_item_count+1));
                    assert(double_list_to_insert!=NULL);
                    double_list_to_insert[double_item_count] = (double)double_val;

                    double_item_count++;
                    break;
                case STRING:
                    // no need to convert type, data_list is stored as string
                    // check pk case
                    if(current_col->constraint == UNIQUE){
                        if(!is_unique_str(table, col_list[i], data_list[i])) {
                            free_insert_before_exit(&int_list_to_insert, &str_list_to_insert, &double_list_to_insert, str_item_count);
                            return;
                        }
                    }

                    // expand temp list and store validated value
                    str_list_to_insert = (char*)realloc(str_list_to_insert, sizeof(char) * (strlen(data_list[i])+1));
                    assert(str_list_to_insert!=NULL);
                    str_list_to_insert[str_item_count] = data_list[i];
                    
                    str_item_count++;
                    break;
                default:
                    fprintf(stderr, "Execution error: unknown type for column '%s'.\n", col_list[i]);
                    break;
                }



                // TODO: check PK case, can't insert value already existing, should create a helper for future UNIQUE constraint
                // hash 
            }

            current_col = current_col->next_col;
        }

        if(!col_exist){
            fprintf(stderr, "Column '%s' not found.\n", col_list[i]);
            free_insert_before_exit(&int_list_to_insert, &str_list_to_insert, &double_list_to_insert, str_item_count);
            return;
        }
    



    }

    // start inserting

    //TODO : set id, check id, auto increment (see ideas in README)
    // TODO : hash id then add to hash table of this table
    // TODO : cast data fields to its correct type and add to row


}