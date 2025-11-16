/*
last update : 16/11/2025
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

// Fonction  used to get the index of a column in a table
 int get_col_list_index(Table* table, Row* row, const char* col_name, ColType* col_type) {
    if (table == NULL || row == NULL || col_name == NULL) return -1;

    Col* current_col = table->first_col;
    int int_index = 0;
    int str_index = 0;
    int double_index = 0;

    while (current_col != NULL) {
        if (strcmp(current_col->name, col_name) == 0) {
            *col_type = current_col->type;

            // Return the index of the column
            switch (current_col->type) {
                case INT:
                case BOOL: // BOOL stocké comme int
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

        // updating the index
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
            printf("%.2f", *(float*)value);
            break;
        case DOUBLE:
            printf("%.4f", *(double*)value);
            break;
        case STRING:
            printf("%s", (char*)value);
            break;
        case BOOL:
            printf("%s", *(bool*)value ? "true" : "false");
            break;
        default:
            printf("UNKNOWN");
    }
}

// used to find a table by its name
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

// used to find a column by its name
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
// used to display the result of a SELECT query
 void select_data(Query* query) {
    SelectParams* params = &query->params.select_params;

    // making sure the table exists
    Table* table = find_table(params->table_name);
    if (table == NULL) {
        fprintf(stderr, "Execution error: table '%s' does not exist.\n", params->table_name);
        return;
    }

    // verifying that all columns exist if any are specified
    if (params->col_count > 0 && !(params->col_count == 1 && strcmp(params->col_list[0], "*") == 0)) {
        for (int i = 0; i < params->col_count; i++) {
            if (find_col(table, params->col_list[i]) == NULL) {
                fprintf(stderr, "Execution error: column '%s' does not exist.\n", params->col_list[i]);
                return;
            }
        }
    }

    printf("\n");

    if (params->col_count == 1 && strcmp(params->col_list[0], "*") == 0) {
        // when  select * is used, display all columns in the table
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
        // when select col1, col2,... is used, display only the specified columns
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
    Row* current_row = table->first_row;
    int row_count = 0;

    while (current_row != NULL) {
        printf("|");

        if (params->col_count == 1 && strcmp(params->col_list[0], "*") == 0) {
            // SELECT *: display all columns in the table
            Col* current_col = table->first_col;
            while (current_col != NULL) {
                ColType col_type;
                void* value = get_col_value(table, current_row, current_col->name, &col_type);
                format_value(col_type, value);
                printf(" |");
                current_col = current_col->next_col;
            }
        } else {
            // SELECT col1, col2: display only the specified columns
            for (int i = 0; i < params->col_count; i++) {
                ColType col_type;
                void* value = get_col_value(table, current_row, params->col_list[i], &col_type);
                format_value(col_type, value);
                printf(" |");
            }
        }

        printf("\n");
        row_count++;
        current_row = current_row->next_row;
    }

    // print the number of rows found
    printf("\nFound %d row(s)\n", row_count);
}