/*
Date of creation : 08/11/2025 (updated 15/11/2025)
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
#include "../global_var.h"
#include "../clean/clean.h"

// Helper function to find primary key column
static Col* get_pk_column(Table* table) {
    Col* current_col = table->first_col;
    while (current_col) {
        if (current_col->constraint == PK) {
            return current_col;
        }
        current_col = current_col->next_col;
    }
    return NULL;
}

// Helper function to get value from row at given column index
static char* get_row_value_at_index(Row* row, Table* table, int col_index, ColType type) {
    static char buffer[512];
    
    switch (type) {
        case INT:
            if (row->int_list && row->int_list[col_index]) {
                snprintf(buffer, sizeof(buffer), "%d", *(row->int_list[col_index]));
                return buffer;
            }
            break;
        case DOUBLE:
            if (row->double_list && row->double_list[col_index]) {
                snprintf(buffer, sizeof(buffer), "%f", *(row->double_list[col_index]));
                return buffer;
            }
            break;
        case STRING:
            if (row->str_list && row->str_list[col_index]) {
                return row->str_list[col_index];
            }
            break;
    }
    return NULL;
}

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

    // Get primary key column for hash table management
    Col* pk_col = get_pk_column(table);
    int pk_col_index = -1;
    if (pk_col) {
        Col* current_col = table->first_col;
        int idx = 0;
        while (current_col) {
            if (strcmp(current_col->name, pk_col->name) == 0) {
                pk_col_index = idx;
                break;
            }
            idx++;
            current_col = current_col->next_col;
        }
    }

    // If WHERE clause exists, check if column exists and get its type
    Col* condition_column = NULL;
    ColType condition_type;
    int condition_col_index = -1;
    if (condition_col) {
        condition_column = get_col_by_name(table, condition_col);
        if (!condition_column) {
            fprintf(stderr, "Execution error: column '%s' not found in table '%s'.\n", condition_col, table_name);
            return;
        }
        condition_type = condition_column->type;
        
        // Find column index
        Col* current_col = table->first_col;
        int idx = 0;
        while (current_col) {
            if (strcmp(current_col->name, condition_col) == 0) {
                condition_col_index = idx;
                break;
            }
            idx++;
            current_col = current_col->next_col;
        }
    }

    // Variables for counting deleted rows
    int deleted_count = 0;
    Row* current_row = table->first_row;
    Row* prev_row = NULL;

    // Delete rows
    while (current_row) {
        int should_delete = 0;

        // If no WHERE clause, delete all rows
        if (!condition_col) {
            should_delete = 1;
        } 
        // If WHERE clause, check condition
        else {
            switch (condition_type) {
                case INT:
                    if (current_row->int_list && current_row->int_list[condition_col_index]) {
                        should_delete = (atoi(condition_val) == *(current_row->int_list[condition_col_index]));
                    }
                    break;
                case STRING:
                    if (current_row->str_list && current_row->str_list[condition_col_index]) {
                        should_delete = (strcmp(condition_val, current_row->str_list[condition_col_index]) == 0);
                    }
                    break;
                case DOUBLE:
                    if (current_row->double_list && current_row->double_list[condition_col_index]) {
                        should_delete = (atof(condition_val) == *(current_row->double_list[condition_col_index]));
                    }
                    break;
            }
        }

        if (should_delete) {
            Row* to_delete = current_row;
            
            // Check ON DELETE RESTRICT constraint: verify no FK in other tables reference this row's PK
            if (pk_col && pk_col_index >= 0) {
                char* pk_value = get_row_value_at_index(current_row, table, pk_col_index, pk_col->type);
                if (pk_value) {
                    // Check all tables for FK references
                    Table* check_table = first_table;
                    int can_delete = 1;
                    
                    while (check_table) {
                        if (strcmp(check_table->name, table_name) != 0) {
                            Col* check_col = check_table->first_col;
                            while (check_col) {
                                if (check_col->constraint == FK && 
                                    strcmp(check_col->refer_table, table_name) == 0 &&
                                    strcmp(check_col->refer_col, pk_col->name) == 0) {
                                    // This FK references our table, check if value exists in FK column
                                    if (refer_val_exists(pk_value, -1, table_name, pk_col->name)) {
                                        fprintf(stderr, "Execution error: '%s' is referenced by '%s' column of table '%s'.\n", 
                                                pk_value, check_col->name, check_table->name);
                                        can_delete = 0;
                                        break;
                                    }
                                }
                                check_col = check_col->next_col;
                            }
                            if (!can_delete) break;
                        }
                        check_table = check_table->next_table;
                    }
                    
                    if (!can_delete) {
                        return;
                    }
                }
            }
            
            // Update hash table if row has primary key
            if (pk_col && pk_col_index >= 0 && table->first_hash_table) {
                char* pk_value = get_row_value_at_index(current_row, table, pk_col_index, pk_col->type);
                if (pk_value) {
                    unsigned int key;
                    if (pk_col->type == INT) {
                        key = hash_int(atoi(pk_value));
                    } else if (pk_col->type == STRING) {
                        key = hash_string(pk_value);
                    } else {
                        key = hash_string(pk_value);
                    }

                    // Find and remove from all hash tables
                    HashTable* hash_table = table->first_hash_table;
                    while (hash_table) {
                        if (strcmp(hash_table->col_name, pk_col->name) == 0) {
                            // Remove from hash table
                            Node* current_node = hash_table->bucket[key];
                            Node* prev_node = NULL;

                            while (current_node) {
                                if (current_node->row == current_row) {
                                    if (prev_node) {
                                        prev_node->next_node = current_node->next_node;
                                    } else {
                                        hash_table->bucket[key] = current_node->next_node;
                                    }
                                    free_node(current_node);
                                    break;
                                }
                                prev_node = current_node;
                                current_node = current_node->next_node;
                            }
                        }
                        hash_table = hash_table->next_hash_table;
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
            free_row(to_delete);
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