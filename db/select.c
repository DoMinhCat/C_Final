/*
last update : 16/11/2025
Description : Code for select_data function
Group 2 ESGI 2A3
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "db.h"
#include "helper_db.h"
#include "../main.h"
#include "../ui/parser.h"
#include "../init/init.h"
#include "../hash/hash.h"
#include "../global_var.h"
#include "../clean/clean.h"

void print_header_row(bool select_all, Table* table1, Table* table2, SelectParams* params){
    // print the first row (col names passed to select)

    Col* current_col = NULL;

    if (select_all) {
        //  SELECT *
        printf("|");
        for(current_col = table1->first_col; current_col != NULL; current_col = current_col->next_col)
            printf(" %-21s |", current_col->name); 
        if(table2){
            for(current_col = table2->first_col; current_col != NULL; current_col = current_col->next_col) 
                printf(" %-21s |", current_col->name);
        }
        printf("\n");

        printf("|");
        for(current_col = table1->first_col; current_col != NULL; current_col = current_col->next_col) 
            printf("-----------------------|");
        printf("\n");
        if(table2){
            for(current_col = table2->first_col; current_col != NULL; current_col = current_col->next_col) 
            printf("-----------------------|");
        }
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
}

void print_empty_table(bool select_all, Table* table1, Table* table2, SelectParams* params){
    int i;

    printf("|");
    if(select_all){
        for(i=0; i<table2!=NULL?table1->col_count+table2->col_count:table1->col_count; i++) printf("%23s|", " ");
    }else{
        for(int i = 0; i < params->col_count; i++) printf("%23s|", " ");
    }
    printf("\n\n");
    printf("Found 0 row.\n");
}

void print_data(bool select_all, Table* table, Row* current_row, SelectParams* params){
    // print out the selected data, handles both select * and select col1, col2,...
    Col* current_col = NULL;
    ColType col_type;

    printf("|");
    if (select_all) {
        // print all columns in the right order
        for(current_col = table->first_col; current_col != NULL; current_col = current_col->next_col) {
            col_type = current_col->type;
            void* value = get_col_value(table, current_row, current_col->name, col_type);
            format_value(col_type, value);
            printf("|");
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
}

void print_data_for_join(FilteredRow* filtered_set, SelectedColInfo* col_info){
    // print the filtered list after JOIN operation

    FilteredRow* current_fr = NULL;
    Col* current_col = NULL;
    ColType col_type;
    int i;

    printf("|");
    for(current_fr = filtered_set; current_fr != NULL; current_fr = current_fr->next_filtered_row) {
        col_type = current_col->type;
        void* value = get_col_value(table, current_row, current_col->name, col_type);
        format_value(col_type, value);
        printf("|");   
    }
    printf("\n");
}

void select_simple(SelectParams* params, Table* table){
    // select without JOIN and WHERE: SELECT */col1, col2,... from tab1

    bool select_all = params->col_count == 1 && strcmp(params->col_list[0], "*") == 0;
    Col* current_col = NULL;
    ColType col_type;

    print_header_row(select_all, table, NULL, params);

    // starts printing data
    Row* current_row = table->first_row;
    int row_count = 0;

    // if table is empty
    if(!table->first_row){
        print_empty_table(select_all, table, NULL, params);
        return;
    }

    // table is not empty
    while (current_row != NULL) {
        print_data(select_all, table, current_row, params);
        
        row_count++;
        current_row = current_row->next_row;
    }

    // print number of rows
    printf("\nFound %d %s.\n", row_count, row_count>1?"rows":"row");
}

void select_where_only(SelectParams* params, Table* table){
    // select with WHERE
    
    FilteredRow* filtered = NULL;
    Col* condition_col = get_col_by_name(table, params->condition_col);
    char* condition_val = params->condition_val;
    ColType col_type = condition_col->type;
    Row* current_row = NULL;
    bool select_all = params->col_count == 1 && strcmp(params->col_list[0], "*") == 0;
    int row_count = 0;

    int int_val;
    double double_val;
    char* str_val = NULL;
    
    // NOTE: could make a function for "type conversion" if repititive across select cases
    // special case compare with NULL
    if(strcasecmp(params->condition_val, "NULL") == 0){
        filtered = where_for_select(table, condition_col, "NULL", 0, 0, col_type);
    } else{
        // convert condition value to the correct type to compare
        switch (col_type) {
        case INT:
            if(!str_to_int(condition_val, &int_val, condition_col->name)) return;
            break;
        case DOUBLE:
            if(!str_to_double(condition_val, &double_val, condition_col->name)) return;
            break;
        case STRING:
            str_val = condition_val;
            break;
        default:
            break;
        }

        filtered = where_for_select(table, condition_col, str_val, double_val, int_val, col_type);
    }

    // print results
    print_header_row(select_all, table, NULL, params);
    if(!filtered){
        print_empty_table(select_all, table, NULL, params);
        return;
    }

    while(filtered){
        current_row = filtered->row;
        print_data(select_all, table, current_row, params);

        row_count++;
        filtered = filtered->next_filtered_row;
    }

    // free before exit
    free_filtered_set(filtered);
    printf("\nFound %d %s.\n", row_count, row_count>1?"rows":"row");
}

void select_join_only(Table* tab1, Table* tab2, SelectParams* params, SelectedColInfo* col_info){
    // select with JOIN

    FilteredRow* filtered = NULL;
    Row* current_row = NULL;
    Col* col1 = params->first_col_on;
    Col* col2 = params->second_col_on;
    bool select_all = params->col_count == 1 && strcmp(params->col_list[0], "*") == 0;
    int row_count = 0;

    filtered = join(tab1, tab2, col1, col2, params);

    print_header_row(select_all, tab1, tab2, params);
    if(!filtered){
        print_empty_table(select_all, tab1, tab2, params);
        return;
    }

    // TODO: func for print data join
    while(filtered){
        current_row = filtered->row;
        print_data_for_join(filtered, col_info);

        row_count++;
        filtered = filtered->next_filtered_row;
    }

    free_filtered_set(filtered);
    printf("\nFound %d %s.\n", row_count, row_count>1?"rows":"row");
}

void select(Query* query) {
    // main function of select, perform checks and call to select of specfic cases

    SelectParams* params = &query->params.select_params;
    bool select_all = params->col_count == 1 && strcmp(params->col_list[0], "*") == 0;
    bool include_join = false;
    bool include_where = false;
    Table* table = NULL;
    Table* join_table = NULL;
    Table* where_table = NULL;
    int i;
    SelectedColInfo* output_col_info = NULL;
    int col_info_list_size;

    // Make sure table exists
    table = get_table_by_name(params->table_name);
    if (table == NULL) {
        fprintf(stderr, "Execution error: '%s' table not found.\n", params->table_name);
        return;
    }

    // check join params
    if(params->table_join_name){
        Col* col_on1 = NULL;
        Col* col_on2 = NULL;
        include_join = true;
        
        // join table exists ?
        join_table = get_table_by_name(params->table_join_name);
        if(!join_table){
            fprintf(stderr, "Execution error: '%s' table not found.\n", params->table_join_name);
            return;
        }
        // join columns exist ?
        col_on1 = get_col_by_name(table, params->first_col_on);
        col_on2 = get_col_by_name(join_table, params->second_col_on);
        if(!col_on1 || !col_on2) return;
        // join columns same type?
        if(col_on1->type != col_on2->type){
            fprintf(stderr, "Execution error: '%s' and '%s' columns have different types.\n", col_on1->name, col_on2->name);
            return;
        }
    }

    //make sure all selected columns exist
    if (params->col_count > 0 && !select_all) {
        for (i = 0; i < params->col_count; i++) {
            if (!get_col_by_name(table, params->col_list[i])) {
                if(include_join){
                    if(!get_col_by_name(join_table, params->col_list[i])){
                        fprintf(stderr, "Execution error: '%s' column  not found.\n", params->col_list[i]);
                        return;
                    }
                }else{
                    fprintf(stderr, "Execution error: '%s' column  not found.\n", params->col_list[i]);
                    return;
                }
            }
        }
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

    // call to select of each case
    if (include_join && include_where) {
        //select_join_where(params);
        return;
    }
    if (include_join) {
        if(select_all) col_info_list_size = table->col_count + join_table->col_count;
        else col_info_list_size = params->col_count;
        output_col_info = build_col_info_list(table, join_table, params, col_info_list_size);

        select_join_only(table, join_table, params, output_col_info);

        // free dynamic col_output_info list
        free(output_col_info);
        // already freed filtered set in select_join_only
        return;
    }
    if (include_where) {
        select_where_only(params, table);
        // already freed filtered set in select_where_only
        return;
    }
    select_simple(params, table);
}
