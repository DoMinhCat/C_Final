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

void select_simple(SelectParams* params, Table* table){
    // select without JOIN and WHERE: SELECT */col1, col2,... from tab1

    bool select_all = params->col_count == 1 && strcmp(params->col_list[0], "*") == 0;
    Col* current_col = NULL;
    ColType col_type;
    int i;

    // print header row (column names)
    if (select_all) {
        //  SELECT *
        printf("|");
        current_col = table->first_col;
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
        printf("\n\n");
        printf("Found 0 row.\n");
        return;
    }

    // table is not empty
    while (current_row != NULL) {
        printf("|");

        current_col = table->first_col;
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

void select(Query* query) {
    // main function of select, perform checks and call to select of specfic cases

    SelectParams* params = &query->params.select_params;
    bool select_all = params->col_count == 1 && strcmp(params->col_list[0], "*") == 0;


    bool include_join = false;
    bool include_where = false;
    ColType col_type;
    Table* table = NULL;
    Table* join_table = NULL;
    Table* where_table = NULL;

    int i;

    // Make sure table exists
    table = get_table_by_name(params->table_name);
    if (table == NULL) {
        fprintf(stderr, "Execution error: '%s' table not found.\n", params->table_name);
        return;
    }

    //make sure all columns exist
    if (params->col_count > 0 && !select_all) {
        for (i = 0; i < params->col_count; i++) {
            if (!col_exists(table, params->col_list[i])) return;
        }
    }

    // check join params
    if(params->table_join_name){
        include_join = true;
        join_table = get_table_by_name(params->table_join_name);
        // join table exists ?
        if(!join_table){
            fprintf(stderr, "Execution error: '%s' table not found.\n", params->table_join_name);
            return;
        }
        // join columns exist ?
        if(!col_exists(table, params->first_col_on) || !col_exists(join_table, params->second_col_on)) return;
    }

    // check where params
    if (params->condition_col) {
        include_where = true;
        where_table = table;

        if (!get_col_by_name(table, params->condition_col)) {
            if (include_join) {
                if (!get_col_by_name(join_table, params->condition_col)) {
                    fprintf(stderr, "Execution error: '%s' column  not found.\n", params->condition_col);
                    return;
                }
                where_table = join_table;
            } else{
                fprintf(stderr, "Execution error: '%s' column  not found.\n", params->condition_col);
                return;
            }
        }
    }
    printf("\n");

    if (include_join && include_where) {
        select_join_where(params);
        return;
    }

    if (include_join) {
        select_join_only(params);
        return;
    }

    if (include_where) {
        select_where_only(params);
        return;
    }

    select_simple(params, table);
    
}