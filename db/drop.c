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
//#include "helper_db.h"  dont need this for now
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

    int i;
    int found = -1; // not found by default
    int table_count = query->params.drop_params.table_count;
    
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

                table_count--;

                res->status = SUCCESS;
                sprintf(res->message, "Table '%s' dropped successfully.", table_name);
                return res;
            }
            // go to next table to check
            prev_table = current_table;
            current_table = current_table->next_table;
        }

        // Check if any other table has a foreign key reference to this table
        check_table = first_table;
        while(check_table != NULL) {
            check_col = check_table->first_col;
            while(check_col != NULL) {
                if(check_col->constraint == FK) {
                    // We need to check if this FK points to our table
                    // However, the reference information is only available during creation
                    check_col = check_col->next_col;
                    continue;
                }
                check_col = check_col->next_col;
            }
            check_table = check_table->next_table;
        }

        // loop through all table, didn't find the current table
        res->status = FAILURE;
        sprintf(res->message, "Execution error: table '%s' not found.", table_name);
        return res;
    }
}