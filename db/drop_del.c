#include "db.h"
#include "../global_var.h"

#include <string.h>
#include <stdlib.h>

// first table already set in global_var.c

// could move to helper.h
// temporary ?
Table* get_first_table(void) {
    return first_table;
}

// could move to helper.h
// temporary ? 
void set_first_table(Table* table) {
    first_table = table;
}

// could move to helper.h
Table* get_table(const char* table_name) {
    Table* current = first_table;
    while (current != NULL) {
        if (strcmp(current->name, table_name) == 0) {
            return current;
        }
        current = current->next_table;
    }
    return NULL;
}

// TODO : delete_rows() and drop_table() should return the structure Response
bool delete_rows(Table* table, const char* column_name, const char* value) {
    if (!table || !column_name || !value) return false;

    Row* current = table->first_row;
    Row* prev = NULL;
    Col* column = table->first_col;
    int col_index = 0;

    while (column && strcmp(column->name, column_name) != 0) {
        column = column->next_col;
        col_index++;
    }
    if (!column) return false;

    while (current != NULL) {
        bool should_delete = false;
        void* field_value = current->data_field[col_index];

        switch (column->type) {
            case STRING:
                should_delete = strcmp((char*)field_value, value) == 0;
                break;
            case INT:
                should_delete = (*(int*)field_value == atoi(value));
                break;
            case DOUBLE:
                should_delete = (*(double*)field_value == atof(value));
                break;
        }

        if (should_delete) {
            Row* to_delete = current;
            if (prev == NULL) {
                table->first_row = current->next_row;
            } else {
                prev->next_row = current->next_row;
            }
            current = current->next_row;
            free(to_delete->data_field);
            free(to_delete);
        } else {
            prev = current;
            current = current->next_row;
        }
    }
    return true;
}

// TODO : delete_rows() and drop_table() should return the structure Response
bool drop_table(const char* table_name) {
    if (!table_name) return false;

    Table* current = first_table;
    Table* prev = NULL;

    while (current != NULL) {
        if (strcmp(current->name, table_name) == 0) {
            if (prev == NULL) {
                first_table = current->next_table;
            } else {
                prev->next_table = current->next_table;
            }

            Row* current_row = current->first_row;
            while (current_row != NULL) {
                Row* to_delete = current_row;
                current_row = current_row->next_row;
                free(to_delete->data_field);
                free(to_delete);
            }

            Col* current_col = current->first_col;
            while (current_col != NULL) {
                Col* to_delete = current_col;
                current_col = current_col->next_col;
                free(to_delete);
            }

            free(current->name);
            free(current);
            return true;
        }
        prev = current;
        current = current->next_table;
    }
    return false;
}