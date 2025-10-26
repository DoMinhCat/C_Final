/*
Date of creation : 16/10/2025
Description : Definition of functions parsing user's commands
Group 2 ESGI 2A3
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../main.h"
#include "../db/db.h"
#include "parser.h"

char* read_cmd(char* cmd_buffer){
    char* cmd_string;
    scanf("%s", cmd_buffer);
    assert((cmd_string = (char*)malloc(sizeof(char)*strlen(cmd_buffer)+1)) != NULL);
    strcpy(cmd_string, cmd_buffer);
    return cmd_string;
}

Query* parse_command(char* cmd) {
    Query* query = malloc(sizeof(Query));
    char* token;
    char* rest = cmd;
    char cmd_copy[MAX_CMD_SIZE];
    strcpy(cmd_copy, cmd);

    token = strtok(rest, " ");
    if (token == NULL) return NULL;

    if (strcmp(token, "DELETE") == 0) {
        query->cmd_type = DELETE;
        token = strtok(NULL, " ");
        if (token && strcmp(token, "FROM") == 0) {
            token = strtok(NULL, " \n");
            if (token) {
                strcpy(query->params.delete_params.table_name, token);
                token = strtok(NULL, " ");
                if (token && strcmp(token, "WHERE") == 0) {
                    token = strtok(NULL, " =");
                    if (token) {
                        strcpy(query->params.delete_params.condition_column, token);
                        token = strtok(NULL, " ");
                        if (token) {
                            strcpy(query->params.delete_params.condition_value, token);
                        }
                    }
                }
            }
        }
    }
    else if (strcmp(token, "DROP") == 0) {
        query->cmd_type = DROP;
        token = strtok(NULL, " ");
        if (token && strcmp(token, "TABLE") == 0) {
            token = strtok(NULL, " \n");
            if (token) {
                strcpy(query->params.drop_params.table_name, token);
            }
        }
    }
    else {
        free(query);
        return NULL;
    }

    return query;
}

// TODO: not necessary, see workflow sent in Discord
// void execute_delete(Query* query) {
//     Table* current_table = get_table(query->params.delete_params.table_name);
//     if (current_table == NULL) return;

//     Row* current = current_table->first_row;
//     Row* prev = NULL;
//     Col* column = current_table->first_col;
//     int col_index = 0;

//     while (column && strcmp(column->name, query->params.delete_params.condition_column) != 0) {
//         column = column->next_col;
//         col_index++;
//     }

//     while (current != NULL) {
//         int should_delete = 0;
//         char* str_value;
//         int int_value;
//         double double_value;

//         switch (column->type) {
//             case STRING:
//                 str_value = (char*)current->data_field[col_index];
//                 should_delete = strcmp(str_value, query->params.delete_params.condition_value) == 0;
//                 break;
//             case INT:
//                 int_value = *(int*)current->data_field[col_index];
//                 should_delete = int_value == atoi(query->params.delete_params.condition_value);
//                 break;
//             case DOUBLE:
//                 double_value = *(double*)current->data_field[col_index];
//                 should_delete = double_value == atof(query->params.delete_params.condition_value);
//                 break;
//         }

//         if (should_delete) {
//             Row* to_delete = current;
//             if (prev == NULL) {
//                 current_table->first_row = current->next_row;
//             } else {
//                 prev->next_row = current->next_row;
//             }
//             current = current->next_row;
//             free(to_delete->data_field);
//             free(to_delete);
//         } else {
//             prev = current;
//             current = current->next_row;
//         }
//     }
// }

// TODO: not necessary, see workflow sent in Discord
// void execute_drop(Query* query) {
//     Table* current = get_first_table();
//     Table* prev = NULL;

//     while (current != NULL) {
//         if (strcmp(current->name, query->params.drop_params.table_name) == 0) {
//             if (prev == NULL) {
//                 set_first_table(current->next_table);
//             } else {
//                 prev->next_table = current->next_table;
//             }

//             Row* current_row = current->first_row;
//             while (current_row != NULL) {
//                 Row* to_delete = current_row;
//                 current_row = current_row->next_row;
//                 free(to_delete->data_field);
//                 free(to_delete);
//             }

//             Col* current_col = current->first_col;
//             while (current_col != NULL) {
//                 Col* to_delete = current_col;
//                 current_col = current_col->next_col;
//                 free(to_delete);
//             }

//             free(current->name);
//             free(current);
//             return;
//         }
//         prev = current;
//         current = current->next_table;
//     }
// }