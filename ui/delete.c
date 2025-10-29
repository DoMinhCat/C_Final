/*
Date of creation : 27/10/2025
Description : parse_delete to analyse delete command
Group 2 ESGI 2A3
*/

#include <string.h>
#include <stdio.h>
#include "parser.h"

void parse_delete(Query** query){
    char* token;

    (*query)->cmd_type = DELETE;

    // check FROM
    token = strtok(NULL, " \t");
    if (!token || strcasecmp(token, "FROM") != 0) {
        (*query)->cmd_type = INVALID;
        sprintf((*query)->syntax_message, "Syntax error: expected 'FROM' after DELETE.");
        return;
    }

    // check table name
    token = strtok(NULL, " \n");
    if (!token || strlen(token) == 0) {
        (*query)->cmd_type = INVALID;
        sprintf((*query)->syntax_message, "Syntax error: missing table name after FROM.");
        return;
    }

    // get table name
    strncpy((*query)->params.delete_params.table_name, token, sizeof((*query)->params.delete_params.table_name) - 1);

    // get WHERE (optional)
    token = strtok(NULL, " \t");
    if (token) {
        if(strcasecmp(token, "WHERE") == 0){
            // get col name
            token = strtok(NULL, " \t");
            if (!token || strlen(token) == 0) {
                (*query)->cmd_type = INVALID;
                sprintf((*query)->syntax_message, "Syntax error: missing column name in WHERE clause.");
                return;
            }
            strncpy((*query)->params.delete_params.condition_column, token, sizeof((*query)->params.delete_params.condition_column) - 1);
            
            // get "="
            token = strtok(NULL, " \t");
            if (!token) {
                (*query)->cmd_type = INVALID;
                sprintf((*query)->syntax_message, "Syntax error: missing '=' after '%s'.", (*query)->params.delete_params.condition_column);
                return;
            }
            if (strcmp(token, "=") != 0) {
                (*query)->cmd_type = INVALID;
                sprintf((*query)->syntax_message, "Syntax error: command '%s' not found, please check the syntax.", token);
                return;
            }

            // get condition value
            token = strtok(NULL, " \n\t");
            if (!token || strlen(token) == 0) {
                (*query)->cmd_type = INVALID;
                sprintf((*query)->syntax_message, "Syntax error: missing value in WHERE clause.");
                return;
            }
            // strip newline from fgets because this is the end of cmd when user hit enter
            token[strcspn(token, "\n")] = '\0';
            strncpy((*query)->params.delete_params.condition_value, token, sizeof((*query)->params.delete_params.condition_value) - 1);
        } else{
            (*query)->cmd_type = INVALID;
            sprintf((*query)->syntax_message, "Syntax error : command '%s' not found, please check the syntax.", token);
            return;
        }
    }
}
