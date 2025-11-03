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

Response* drop_table(Query* query) {
    Response* res = init_response();
    Table* current_table = first_table;
    Table* prev_table = NULL;
    
    // Use the first table name in the list
    if (query->params.drop_params.table_count < 1) {
        strcpy(res->message, "No table name provided for DROP operation");
        res->status = FAILURE;
        return res;
    }
    char* table_name = query->params.drop_params.table_list[0];

    // Check if any other table has a foreign key reference to this table
    Table* check_table = first_table;
    while(check_table != NULL) {
        Col* check_col = check_table->first_col;
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

    // Find the table to drop
    while(current_table != NULL) {
        if(strcmp(current_table->name, table_name) == 0) {
            // First, free all rows
            Row* current_row = current_table->first_row;
            while(current_row != NULL) {
                Row* next_row = current_row->next_row;
                free(current_row->data_field);
                free(current_row);
                current_row = next_row;
            }

            // Free all columns
            Col* current_col = current_table->first_col;
            while(current_col != NULL) {
                Col* next_col = current_col->next_col;
                free(current_col->name);
                free(current_col);
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
        prev_table = current_table;
        current_table = current_table->next_table;
    }

    res->status = FAILURE;
    sprintf(res->message, "Execution error: table '%s' does not exist.", table_name);
    return res;
}