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
    bool select_all = params->col_count == 1 && strcmp(params->col_list[0], "*") == 0;
    ColType col_type;

    int i;

    // Make sure table exists
    Table* table = get_table_by_name(params->table_name);
    if (table == NULL) {
        fprintf(stderr, "Execution error: '%s' table not found.\n", params->table_name);
        return;
    }

    //make sure all columns exist
    if (params->col_count > 0 && !select_all) {
        for (int i = 0; i < params->col_count; i++) {
            if (get_col_by_name(table, params->col_list[i]) == NULL) {
                fprintf(stderr, "Execution error: '%s' column not found.\n", params->col_list[i]);
                return;
            }
        }
    }
    printf("\n");

    // print header row (column names)
    if (select_all) {
        //  SELECT *
        printf("|");
        Col* current_col = table->first_col;
        while (current_col != NULL) {
            printf(" %-21s |", current_col->name);
            current_col = current_col->next_col;
        }
        printf("\n");

    printf("|");
        current_col = table->first_col;
        while (current_col != NULL) {
            printf("-----------------------|");
            current_col = current_col->next_col;
        }
        printf("\n");
    } else {
        // SELECT col1, col2, ... :/
        printf("|");
        for (int i = 0; i < params->col_count; i++) {
            printf(" %-21s |", params->col_list[i]);
        }
        printf("\n");


        printf("|");
        for (int i = 0; i < params->col_count; i++) {
            printf("-----------------------|");
        }
        printf("\n");
    }

    // print data
    Row* current_row = table->first_row;
    int row_count = 0;

    // if table is empty
    if(!table->first_row){
        printf("|");
        if(select_all){
            for(i=0; i<table->col_count; i++) printf("%23s|", " ");
        }else{
            for(int i = 0; i < params->col_count; i++) printf("%23s|", " ");
        }
        printf("\n");
        printf("Found 0 row.\n");
        return;
    }

    // table is not empty
    while (current_row != NULL) {
        printf("|");

        Col* current_col = table->first_col;
        if (select_all) {
            // print all columns in the right order
            while (current_col != NULL) {
                col_type = current_col->type;
                void* value = get_col_value(table, current_row, current_col->name, col_type);
                format_value(col_type, value);
                printf("|");
                current_col = current_col->next_col;
            }
        } else {
            // SELECT col1, col2: print the right columns
            for (int i = 0; i < params->col_count; i++) {
                col_type = get_col_by_name(table, params->col_list[i])->type;
                void* value = get_col_value(table, current_row, params->col_list[i], col_type);
                format_value(col_type, value);
                printf("|");
            }
        }

        printf("\n");
        row_count++;
        current_row = current_row->next_row;
    }

    // print number of rows
    printf("\nFound %d %s.\n", row_count, row_count>1?"rows":"row");
}