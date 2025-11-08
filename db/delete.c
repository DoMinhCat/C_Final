/*
Date of creation : 08/11/2025
Description : Code for DELETE operation
Group 2 ESGI 2A3
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "db.h"
#include "helper_db.h"
#include "../ui/parser.h"
#include "../hash/hash.h"
#include "../clean/clean.h"

void delete_from_table(Query* query) {
    char* table_name = query->params.delete_params.table_name;
    char* condition_col = query->params.delete_params.condition_column;
    char* condition_val = query->params.delete_params.condition_value;
    
    // Get the table
    Table* table = get_table_by_name(table_name);
    if (!table) {
        fprintf(stderr, "Execution error: table '%s' not found.\n", table_name);
        return;
    }

    // If WHERE clause exists, check if column exists and get its type
    Col* condition_column = NULL;
    ColType condition_type;
    if (condition_col) {
        condition_column = get_col_by_name(table, condition_col);
        if (!condition_column) {
            fprintf(stderr, "Execution error: column '%s' not found in table '%s'.\n", condition_col, table_name);
            return;
        }
        condition_type = condition_column->type;
    }

    // Variables for counting deleted rows
    int deleted_count = 0;
    Row* current_row = table->first_row;
    Row* prev_row = NULL;
    int col_index = 0;

    // If WHERE clause exists, find the column index
    if (condition_col) {
        Col* current_col = table->first_col;
        while (current_col && strcmp(current_col->name, condition_col) != 0) {
            col_index++;
            current_col = current_col->next_col;
        }
    }

    // Delete rows
    while (current_row) {
        bool should_delete = false;

        // If no WHERE clause, delete all rows
        if (!condition_col) {
            should_delete = true;
        } 
        // If WHERE clause, check condition
        else if (current_row->data_field && current_row->data_field[col_index]) {
            switch (condition_type) {
                case INT:
                    should_delete = (atoi(condition_val) == *((int*)current_row->data_field[col_index]));
                    break;
                case STRING:
                    should_delete = (strcmp(condition_val, (char*)current_row->data_field[col_index]) == 0);
                    break;
                case DOUBLE:
                    should_delete = (atof(condition_val) == *((double*)current_row->data_field[col_index]));
                    break;
            }
        }

        if (should_delete) {
            Row* to_delete = current_row;
            
            // Update hash table if row has primary key
            if (table->hash_table) {
                Col* current_col = table->first_col;
                int pk_index = 0;
                // Find primary key column index
                while (current_col && strcmp(current_col->name, table->hash_table->pk_col_name) != 0) {
                    pk_index++;
                    current_col = current_col->next_col;
                }
                
                if (current_row->data_field[pk_index]) {
                    unsigned int key;
                    if (current_col->type == INT) {
                        key = hash_int(*((int*)current_row->data_field[pk_index]));
                    } else if (current_col->type == STRING) {
                        key = hash_string((char*)current_row->data_field[pk_index]);
                    }

                    // Remove from hash table
                    Node* current_node = table->hash_table->bucket[key];
                    Node* prev_node = NULL;

                    while (current_node) {
                        if (current_node->row == current_row) {
                            if (prev_node) {
                                prev_node->next_node = current_node->next_node;
                            } else {
                                table->hash_table->bucket[key] = current_node->next_node;
                            }
                            free_node(current_node, table->col_count);
                            break;
                        }
                        prev_node = current_node;
                        current_node = current_node->next_node;
                    }
                }
            }

            // Update linked list
            if (prev_row) {
                prev_row->next_row = current_row->next_row;
            } else {
                table->first_row = current_row->next_row;
            }

            current_row = current_row->next_row;
            free_row(to_delete, table->col_count);
            deleted_count++;
        } else {
            prev_row = current_row;
            current_row = current_row->next_row;
        }
    }

    // Print success message
    if (condition_col) {
        fprintf(stdout, "%d row(s) deleted from table '%s' where %s = '%s'.\n", 
                deleted_count, table_name, condition_col, condition_val);
    } else {
        fprintf(stdout, "%d row(s) deleted from table '%s'.\n", deleted_count, table_name);
    }
}