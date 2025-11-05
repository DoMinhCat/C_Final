/*
last update  :
Description : Code for select_data function
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
#include "../hash/hash.h"
#include "../global_var.h"

int get_col_index(Table* table, const char* col_name) {
if (table == NULL || col_name == NULL) return -1;

Col* current = table->first_col;
int index = 0;
while (current != NULL) {
    if (strcmp(current->name, col_name) == 0) {
        return index;
    }
    current = current->next_col;
    index++;
}
return -1;
}

bool compare_values(ColType type, void* value1, const char* value2_str) {
if (value1 == NULL || value2_str == NULL) return false;

switch (type) {
    case INT: {
        int val1 = *(int*)value1;
        int val2 = atoi(value2_str);
        return val1 == val2;
    }
    // Cat: we dont have float type
    case DOUBLE: {
        double val1 = *(double*)value1;
        double val2 = atof(value2_str);
        return val1 == val2;
    }
    case STRING: {
        return strcmp((char*)value1, value2_str) == 0;
    }
    // Cat: we dont have bool type
    default:
        return false;
}
}

void format_value(char* buffer, int buffer_size, ColType type, void* value) {
if (value == NULL) {
    strncpy(buffer, "NULL", buffer_size);
    return;
}

switch (type) {
    case INT:
        snprintf(buffer, buffer_size, "%d", *(int*)value);
        break;
    case DOUBLE:
        snprintf(buffer, buffer_size, "%.4f", *(double*)value);
        break;
    case STRING:
        snprintf(buffer, buffer_size, "%s", (char*)value);
        break;
    default:
        strncpy(buffer, "UNKNOWN", buffer_size);
}
}

bool check_where_condition(Table* table, Row* row, const char* condition_col, const char* condition_val) {
    if (condition_col == NULL || condition_val == NULL) return true;

    int col_index = get_col_index(table, condition_col);
    if (col_index == -1) return false;

    Col* col = get_col_by_name(table, condition_col);
    if (col == NULL) return false;

    return compare_values(col->type, row->data_field[col_index], condition_val);
    }

void get_col_value_from_row(Table* table, Row* row, const char* col_name, char* buffer, int buffer_size) {
    if (table == NULL || row == NULL || col_name == NULL) {
        strncpy(buffer, "NULL", buffer_size);
        return;
    }

    int col_index = get_col_index(table, col_name);
    if (col_index == -1) {
        strncpy(buffer, "COL_NOT_FOUND", buffer_size);
        return;
    }

    Col* col = get_col_by_name(table, col_name);
    if (col == NULL) {
        strncpy(buffer, "COL_NOT_FOUND", buffer_size);
        return;
    }

    format_value(buffer, buffer_size, col->type, row->data_field[col_index]);
}

Response* select_data(Query* query) {
    Response* res = init_response();
    SelectParams* params = &query->params.select_params;
    Table* main_table = get_table_by_name(params->table_name);

    if (main_table == NULL) {
        res->status = FAILURE;
        sprintf(res->message, "Execution error: table '%s' does not exist.\n", params->table_name);
        return res;
    }

    Table* join_table = NULL;
    if (params->table_join_name != NULL) {
        join_table = get_table_by_name(params->table_join_name);
        if (join_table == NULL) {
            res->status = FAILURE;
            sprintf(res->message, "Execution error: join table '%s' does not exist.\n", params->table_join_name);
            return res;
        }
    }

    if (params->col_count > 0 && !(params->col_count == 1 && strcmp(params->col_list[0], "*") == 0)) {
        for (int i = 0; i < params->col_count; i++) {
            char* col_name = params->col_list[i];
            bool col_exists = false;

            if (get_col_by_name(main_table, col_name) != NULL) {
                col_exists = true;
            }
            else if (join_table != NULL && get_col_by_name(join_table, col_name) != NULL) {
                col_exists = true;
            }

            if (!col_exists) {
                res->status = FAILURE;
                sprintf(res->message, "Execution error: column '%s' does not exist.\n", col_name);
                return res;
            }
        }
    }
}