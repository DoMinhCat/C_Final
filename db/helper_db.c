/*
Date of creation : 29/10/2025
Description : Helper functions for db core functions
Group 2 ESGI 2A3
*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <float.h>
#include <math.h>

#include "helper_db.h"
#include "../ui/parser.h"
#include "db.h"
#include "../global_var.h"

bool table_exists(char* table_name){
    // check for table existence, print error if not.
    // ex: if(!table_exists(tb_name)) return;
    Table* table = get_table_by_name(table_name);
    if(!table) fprintf(stderr, "Execution error: table '%s' not found.\n", table_name);
}

bool col_exists(Table* table, char* col_name){
    // same as table_exists above but for col
    Col* col = get_col_by_name(table, col_name);
    if(!col) fprintf(stderr, "Execution error: column '%s' not found.\n", col_name);
}

int* get_fk_col_list_index(Query* query){
    /*
    The function returns an array/pointer of index matches with col_refer_list and table_refer_list
    Use this to convert col_refer_list and table_refer_list index to col_list, type_list, or constraint_list

    Example : 
    col_list = col1 pk, col2, col3 fk, col4, col5 fk
    fk_index = 2, 4 -> which are indexes of col3 and col5 in col_list

    Returns NULL pointer if there is error
    */
    int fk_count = query->params.create_params.fk_count;
    int* res = (int*)malloc(sizeof(int) * fk_count);
    assert(res != NULL);
    int current_index = 0;
    int i;

    switch (query->cmd_type)
    {
    case CREATE:
        int col_count = query->params.create_params.col_count;
        ColConstraintType* constraint_list = query->params.create_params.constraint_list;
        for(i=0; i<col_count; i++){
            if(constraint_list[i] == FK){
                res[current_index] = i;
                current_index++;
            }
        }
        break;
    
    default:
        free(res); // will return NULL if error
        break;
    }
    
    // handle an edge case that will probably never happens just to safe guard parse_create() if it has bug :))
    if(current_index != fk_count) free(res);

    return res;
}

Table* get_last_table(Table* first_table){
    // this function assumes there are at least 1 table already
    Table* current_table = first_table;
    
    while(current_table->next_table){
        current_table = current_table->next_table;
    }
    return current_table;
}

Col* get_last_col(Col* first_col){
    // this function assumes there are at least 1 col already
    Col* current_col = first_col;
    
    while(current_col->next_col){
        current_col = current_col->next_col;
    }
    return current_col;
}

Table* get_table_by_name(char* table_name) {
    // this func return pointer to the table having input name
    Table* current = first_table;

    while (current != NULL) {
        if (strcmp(current->name, table_name) == 0) {
            return current;
        }
        current = current->next_table;
    }
    return NULL;
}

Col* get_col_by_name(Table* table, char* col_name) {
    // return pointer to column with input name of a given table
    Col* current = table->first_col;
    
    while (current != NULL) {
        if (strcmp(current->name, col_name) == 0) {
            return current;
        }
        current = current->next_col;
    }
    return NULL;
}

int get_data_list_index(Table* table, char* col_name){
    // get the index of data list of Row, use this to access to data field of row (same as SELECT col1)
    /* ex: 
    col1 int, col2 str, col3 str, col4 int
        0                            1
    get_data_list_index(table, "col4") -> 1 (col4 is INT so col4 is has index 1 for INT col)
    */
    Col* current = table->first_col;
    int i_int = -1;
    int i_str = -1;
    int i_double = -1;
    
    while (current != NULL) {
        if(current->type == INT) i_int++;
        else if(current->type == DOUBLE) i_double++;
        else if(current->type == STRING) i_str++;

        if (strcmp(current->name, col_name) == 0) {
            switch(current->type) {
            case INT:
                return i_int;
                break;
            case STRING:
                return i_str;
                break;
            case DOUBLE:
                return i_double;
                break;
            default:
                return -1; // not gonna happen tho, all col type is INT/STR/DOUBLE
                break;
            }
        }
        current = current->next_col;
    }
    return -1; // if col not found
}

/*
int compare_double(double val1, double val2){
    //safely compare double
    // return 0 if val1=val2
    // return 1 if val1>val2
    // return -1 if val1<val2
    double epsilon = DBL_EPSILON * 10.0;

    if (fabs(val1 - val2) <= epsilon) return 0;
    else if(val1<val2) return -1;
    else return 1;
}*/

bool is_unique_int(Table* table, char* col_name, int value_to_check){
    // check uniqueness of int value of a col
    Row* current = NULL;
    int col_index = get_data_list_index(table, col_name);

    // safe guard, but not likely to happen
    if(col_index == -1) {
        fprintf(stderr, "Execution error: column '%s' not found.\n", col_name);
        return false;
    }

    //loop rows of tables and compare value
    for(current = table->first_row; current!=NULL; current = current->next_row){
        if(value_to_check == current->int_list[col_index]){
            fprintf(stderr, "Execution error: unique constraint violated on column '%s'.\n", col_name);  
            return false;  
        }
    }
    return true;
}

bool is_unique_str(Table* table, char* col_name, char* value_to_check){
    // check uniqueness of str value of a col
    Row* current = NULL;
    int col_index = get_data_list_index(table, col_name);

    if(col_index == -1) {
        fprintf(stderr, "Execution error: column '%s' not found.\n", col_name);
        return false;
    }

    //loop rows of tables and compare value
    for(current = table->first_row; current!=NULL; current = current->next_row){
        if(strcmp(value_to_check, current->str_list[col_index]) == 0 ){
            fprintf(stderr, "Execution error: unique constraint violated on column '%s'.\n", col_name);  
            return false;  
        }
    }
    return true;
}