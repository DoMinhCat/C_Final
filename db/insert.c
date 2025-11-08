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

void insert(Query* query){
    char** col_list = query->params.insert_params.col_list;
    char* col_name = NULL; // col to insert
    int col_count = query->params.insert_params.col_count;
    char** data_list = query->params.insert_params.data_list;
    char* data_field = NULL;
    char* tb_name = query->params.insert_params.table_name;

    Table* table = get_table_by_name(tb_name);
    ColType* type_list = NULL; //(ColType*)malloc(sizeof(ColType) * col_count); // TODO: free at early return
    ColType type;
    Col* current_col = table->first_col;
    bool col_exist;

    // list storing validated field for insert later
    int* int_list_to_insert = NULL;
    char* str_list_to_insert = NULL;
    double* double_list_to_insert = NULL;
    int int_item_count = 0;
    int str_item_count = 0;
    int double_item_count = 0;

    int i,j;

    // check table exist
    if(!table_exists(tb_name)) return;

    // checks for cols to insert
    for(i=0; i<col_count; i++){
        col_name = col_list[i];
        data_field = strdup(data_list[i]); // TODO : free at exit
        assert(data_field!=NULL);

        // Loop through cols of table
        for(j=0; j<table->col_count; j++) {
            col_exist = false;

            // search for col
            if (strcmp(current_col->name, col_name) == 0) {
                col_exist = true;
                type = current_col->type;
                
                // found col, check type col same with data type
                switch (type)
                {
                case INT:
                    char* endptr;
                    long int_val;
                    errno = 0;

                    int_val = strtol(data_field, &endptr, 10);

                    // check conversion error
                    if (endptr == data_field || *endptr != '\0') {
                        fprintf(stderr, "Execution error: invalid value '%s' for column '%s' type INT.\n", data_field, col_name);
                        free(data_field);
                        data_field = NULL;
                        return;
                    }

                    // check overflow for type int
                    if (data_field > INT_MAX || data_field < INT_MIN || errno == ERANGE) {
                        printf("Execution error: incompatible size of value '%s' for type INT.\n");
                        return; 
                    }

                    // if col to insert is pk, check uniqueness
                    if(current_col->type == PK){
                        // TODO
                    }

                    // expand temp list and store validated value
                    int_list_to_insert = (int*)realloc(int_list_to_insert, sizeof(int) * (int_item_count+1));
                    assert(int_list_to_insert!=NULL);
                    int_list_to_insert[int_item_count] = (int)int_val;

                    int_item_count++;
                    break;
                case DOUBLE:
                    /* code */
                    break;
                case STRING:
                    // no need to convert type, data_list is stored as string
                    // check pk case
                    if(current_col->type == PK){
                        // TODO
                    }

                    // expand temp list and store validated value
                    str_list_to_insert = (char*)realloc(str_list_to_insert, sizeof(char) * (strlen(data_field)+1));
                    assert(str_list_to_insert!=NULL);
                    str_list_to_insert[str_item_count] = data_field;
                    
                    str_item_count++;
                    break;
                default:
                    fprintf(stderr, "Execution error: unknown type for column '%s'.\n", col_name);
                    break;
                }



                // TODO: check PK case, can't insert value already existing, should create a helper for future UNIQUE constraint
                // hash 
            }

            current_col = current_col->next_col;
        }

        if(!col_exist){
            fprintf(stderr, "Column '%s' not found.\n", col_name);
            return;
        }
    



    }

    // start inserting

    //TODO : set id, check id, auto increment (see ideas in README)
    // TODO : hash id then add to hash table of this table
    // TODO : cast data fields to its correct type and add to row


}