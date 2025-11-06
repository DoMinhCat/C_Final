/*
Date of creation : 03/11/2025
Description : Code for drop table operation
Group 2 ESGI 2A3
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include "db.h"
#include "helper_db.h"
#include "../main.h"
#include "../ui/parser.h"
#include "../init/init.h"
#include "../global_var.h"
#include "../clean/clean.h"

Response* drop_table(Query* query) {
    Response* res = init_response();
    Table* current_table;
    Table* prev_table = NULL;
    Row* current_row = NULL;
    Row* next_row = NULL;
    Col* current_col = NULL;
    Col* next_col = NULL;
    Table* check_table = NULL;
    Col* check_col = NULL;
    char* table_name = NULL;
    char* table_string = NULL;

    int i;
    int table_count = query->params.drop_params.table_count;
    int table_index[table_count];
    bool table_exist;
    
    // Cat's note : removed check for non existing table, this is already checked in parser

    // Loop through each table provided in query, check and free one by one 
    for(i=0; i<table_count; i++){
        table_name = query->params.drop_params.table_list[i];
        table_exist = false;
    
        // check existence
        current_table = first_table;
        while(current_table != NULL) {
            if(strcmp(table_name, current_table->name) == 0){
                table_exist = true;
                break;
            }
            current_table = current_table->next_table;
        }
        if(!table_exist){
            res->status = FAILURE;
            fprintf(stderr, "Execution error: table '%s' not found.\n", table_name);
            return res;
        }

        // Check if any other table has a foreign key col references to current table
        current_table = first_table;
        while(current_table != NULL) {
            // no need to check itself
            if(strcmp(current_table->name, table_name) == 0){
                current_table = current_table->next_table;
                continue;
            } 
            // loop through all col of the table 
            current_col = current_table->first_col;
            while(current_col != NULL) {
                // return error if col refer to table to delete
                if(current_col->constraint == FK && strcmp(current_col->refer_table, table_name) == 0) {
                    res->status = FAILURE;
                    fprintf(stderr, "Execution error: '%s' 'is referenced by column '%s' of table '%s.\n", table_name, current_col->name, current_table->name);
                    return res;
                }
                current_col = current_col->next_col;
            }
            current_table = current_table->next_table;
        }

        current_table = first_table;
        while(current_table != NULL) {
            if(strcmp(current_table->name, table_name) == 0) {
                // Update the table linked list
                if(prev_table == NULL) {
                    first_table = current_table->next_table;
                } else {
                    prev_table->next_table = current_table->next_table;
                }

                // drop table
                free(current_table);
                current_table = NULL;

                res->status = SUCCESS;
                // construct message 
                table_string = strdup(query->params.drop_params.table_list[0]); // "table"
                assert(table_string != NULL);

                for(i=1; i<table_count; i++){
                    table_name = query->params.drop_params.table_list[i];
                    sprintf(table_string, "'%s', '%s'", table_string, table_name); // "'table','table', ..."
                }
                fprintf(stdout, "Executed: %s %s dropped successfully.\n", table_count>1?"tables":"table", table_string);

                // free before return
                free(table_string);
                table_string = NULL;

                return res;
            }
            // go to next table to check
            prev_table = current_table;
            current_table = current_table->next_table;
        }
    }
}