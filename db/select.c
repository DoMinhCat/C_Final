/*
last update : 16/11/2025
Description : Code for select_data function
Group 2 ESGI 2A3
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "db.h"
#include "helper_db.h"
#include "../main.h"
#include "../ui/parser.h"
#include "../init/init.h"
#include "../hash/hash.h"
#include "../global_var.h"

void select(Query* query) {
    SelectParams* params = &query->params.select_params;

    // Make sure table exists
    Table* table = get_table_by_name(params->table_name);
    if (table == NULL) {
        fprintf(stderr, "Execution error: table '%s' does not exist.\n", params->table_name);
        return;
    }

    //make sure all columns exist
    if (params->col_count > 0 && !(params->col_count == 1 && strcmp(params->col_list[0], "*") == 0)) {
        for (int i = 0; i < params->col_count; i++) {
            if (get_col_by_name(table, params->col_list[i]) == NULL) {
                fprintf(stderr, "Execution error: column '%s' does not exist.\n", params->col_list[i]);
                return;
            }
        }
    }
    printf("\n");

    if (params->col_count == 1 && strcmp(params->col_list[0], "*") == 0) {
        //  SELECT *
        printf("|");
        Col* current_col = table->first_col;
        while (current_col != NULL) {
            printf(" %-15s |", current_col->name);
            current_col = current_col->next_col;
        }
        printf("\n");

    printf("|");
        current_col = table->first_col;
        while (current_col != NULL) {
            printf("-----------------|");
            current_col = current_col->next_col;
        }
        printf("\n");
    } else {
        // SELECT col1, col2, ... :/
        printf("|");
        for (int i = 0; i < params->col_count; i++) {
            printf(" %-15s |", params->col_list[i]);
        }
        printf("\n");


        printf("|");
        for (int i = 0; i < params->col_count; i++) {
            printf("-----------------|");
        }
        printf("\n");
    }

    // print data
    Row* current_row = table->first_row;
    int row_count = 0;

    while (current_row != NULL) {
        printf("|");

        if (params->col_count == 1 && strcmp(params->col_list[0], "*") == 0) {
            // SELECT *:print all columns in the right order
            Col* current_col = table->first_col;
            while (current_col != NULL) {
                ColType col_type;
                void* value = get_col_value(table, current_row, current_col->name, col_type);
                format_value(col_type, value);
                printf(" |");
                current_col = current_col->next_col;
            }
        } else {
            // SELECT col1, col2: print the right columns
            for (int i = 0; i < params->col_count; i++) {
                ColType col_type;
                void* value = get_col_value(table, current_row, params->col_list[i], col_type);
                format_value(col_type, value);
                printf(" |");
            }
        }

        printf("\n");
        row_count++;
        current_row = current_row->next_row;
    }

    // print number of rows
    printf("\nFound %d row(s)\n", row_count);
}