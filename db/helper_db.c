/*
Date of creation : 29/10/2025
Description : Helper functions for db core functions
Group 2 ESGI 2A3
*/

#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "helper_db.h"
#include "../ui/parser.h"
#include "db.h"
#include "../global_var.h"

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

Table* get_table_by_name(const char* table_name) {
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

Col* get_col_by_name(Table* table, const char* col_name) {
    // return pointer to column with input name of a given table
    if (table == NULL || col_name == NULL) return NULL;

    Col* current = table->first_col;
    
    while (current != NULL) {
        if (strcmp(current->name, col_name) == 0) {
            return current;
        }
        current = current->next_col;
    }
    return NULL;
}
// New functions for select.c
Table* find_table(const char* table_name) {
    Table* current = first_table;
    while (current != NULL) {
        if (strcmp(current->name, table_name) == 0) {
            return current;
        }
        current = current->next_table;
    }
    return NULL;
}

Col* find_col(Table* table, const char* col_name) {
    if (table == NULL || col_name == NULL) return NULL;

    Col* current = table->first_col;
    while (current != NULL) {
        if (strcmp(current->name, col_name) == 0) {
            return current;
        }
        current = current->next_col;
    }
    return NULL;
}

int get_col_list_index(Table* table, Row* row, const char* col_name, ColType* col_type) {
    if (table == NULL || row == NULL || col_name == NULL) return -1;

    Col* current_col = table->first_col;
    int int_index = 0;
    int str_index = 0;
    int double_index = 0;

    while (current_col != NULL) {
        if (strcmp(current_col->name, col_name) == 0) {
            *col_type = current_col->type;

            switch (current_col->type) {
                case INT:
                case BOOL:
                    return int_index;
                case STRING:
                    return str_index;
                case FLOAT:
                case DOUBLE:
                    return double_index;
                default:
                    return -1;
            }
        }

        switch (current_col->type) {
            case INT:
            case BOOL:
                int_index++;
                break;
            case STRING:
                str_index++;
                break;
            case FLOAT:
            case DOUBLE:
                double_index++;
                break;
            default:
                break;
        }

        current_col = current_col->next_col;
    }

    return -1;
}

void* get_col_value(Table* table, Row* row, const char* col_name, ColType* col_type) {
    if (table == NULL || row == NULL || col_name == NULL) return NULL;

    int list_index = get_col_list_index(table, row, col_name, col_type);
    if (list_index == -1) return NULL;

    switch (*col_type) {
        case INT:
        case BOOL:
            if (list_index < row->int_count) {
                return &(row->int_list[list_index]);
            }
            break;
        case STRING:
            if (list_index < row->str_count) {
                return row->str_list[list_index];
            }
            break;
        case FLOAT:
        case DOUBLE:
            if (list_index < row->double_count) {
                return &(row->double_list[list_index]);
            }
            break;
        default:
            break;
    }

    return NULL;
}

void format_value(ColType type, void* value) {
    if (value == NULL) {
        printf("NULL");
        return;
    }

    switch (type) {
        case INT:
            printf("%d", *(int*)value);
            break;
        case FLOAT:
            printf("%g", *(float*)value);
            break;
        case DOUBLE:
            printf("%fg", *(double*)value);
            break;
        case STRING:
            printf("%s", (char*)value);
            break;
        case BOOL:
            printf("%s", *(bool*)value ? "true" : "false");
            break;
        default:
            printf("error");
    }
}