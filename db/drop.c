/*
Date of creation : 03/11/2025
Description : Code for drop table operation
Group 2 ESGI 2A3
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "db.h"
#include "helper_db.h"
#include "../main.h"
#include "../ui/parser.h"
#include "../init/init.h"
#include "../global_var.h"
#include "../clean/clean.h"

Response* drop_table(Query* query) {
    Response* res = init_response();
    Table* current_table = first_table;
    Table* prev_table = NULL;
    Row* current_row = NULL;
    Row* next_row = NULL;
    Col* current_col = NULL;
    Col* next_col = NULL;
    Table* check_table = NULL;
    Col* check_col = NULL;
    char* table_name = NULL;
    char* success_msg = NULL;
    char* table_string = NULL;

    int i;
    int found = -1; // not found by default
    int table_count = query->params.drop_params.table_count;
    int table_index[table_count];
    
    // Cat's note : removed check for non existing table, this is already checked in parser

    // Check if all table in the list provided exist
    for(i=0; i<table_count; i++){
        table_name = query->params.drop_params.table_list[i];
        /* Cat's note : maybe I will write a helper function to get the index of the table with a given name
        This function takes the name (and other inputs?), search the list of all tables
        Return position of found table in the list, return -1 if not found
        */

        // table not found, return error
        if(found == -1){
            res->status = FAILURE;
            fprintf(stderr, "Execution error: table '%s' not found.", table_name);
            return res;
        }
    }

    // When we are here, all table provided exist, continue with other checks
    for(i=0; i<table_count; i++){
        table_name = query->params.drop_params.table_list[i];

        // Loop through all tables to find table to drop
        // Cat's note :  maybe dont need to loop all again, we can store indexes of tables given from the check if all tables exist above
        // then free them one by one, no loop -> more efficient
        while(current_table != NULL) {
            if(strcmp(current_table->name, table_name) == 0) {
                // First, free all rows

                current_row = current_table->first_row;
                while(current_row != NULL) {
                    next_row = current_row->next_row;
                    free_row(current_row, current_table->col_count);
                    current_row = next_row;
                }

                // Free all columns
                current_col = current_table->first_col;
                while(current_col != NULL) {
                    next_col = current_col->next_col;
                    free_col(current_col);
                    current_col = next_col;
                }

                // Update the table linked list
                if(prev_table == NULL) {
                    first_table = current_table->next_table;
                } else {
                    prev_table->next_table = current_table->next_table;
                }

                // Free the table name and structure
                free(current_table->name);
                free(current_table);

                res->status = SUCCESS;
                // construct message 
                table_string = strdup(query->params.drop_params.table_list[0]); // "table"
                assert(table_string != NULL);

                for(i=1; i<table_count; i++){
                    table_name = query->params.drop_params.table_list[i];
                    sprintf(table_string, "'%s', '%s'", table_string, table_name); // "'table','table', ..."
                }

                sprintf(success_msg, "%s %s", table_count>1?"tables":"table", table_string); // table(s) 'table','table', ...
                fprintf(stdout, "Executed: %s dropped successfully.\n", success_msg);

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