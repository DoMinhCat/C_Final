/*
Date of creation : 27/10/2025
Description : parse_create to analyse create command
Group 2 ESGI 2A3
*/

#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "parser.h"

void parse_create(Query** query){
    char* token;

    (*query)->cmd_type = CREATE;

    // check TABLE
    token = strtok(NULL, " ");
    if(!token || strcasecmp(token, "TABLE") != 0){
        (*query)->cmd_type = INVALID;
        sprintf((*query)->syntax_message, "Syntax error: missing 'TABLE' after CREATE.");
        return;
    }

    // get table name to create
    token = strtok(NULL, " ");
    if (!token) {
        (*query)->cmd_type = INVALID;
        sprintf((*query)->syntax_message, "Syntax error: missing table name after TABLE.");
        return;
    }
    strncpy((*query)->params.create_params.table_name, token, sizeof((*query)->params.create_params.table_name)-1);
    (*query)->params.create_params.table_name[sizeof((*query)->params.create_params.table_name)-1] = '\0';

    // get column names, type, pk/fk
    char* col_name;
    char* col_type;
    char* col_constraint;
    int i;

    (*query)->params.create_params.col_count = 0;
    token = strtok(NULL, " ,"); // get first column
    while(token != NULL){
        i = (*query)->params.create_params.col_count;

        // increase size of dynamic elements via realloc
        (*query)->params.create_params.col_list = (char**)realloc((*query)->params.create_params.col_list, (i + 1) * sizeof(char*));
        assert((*query)->params.create_params.col_list != NULL);

        (*query)->params.create_params.type_list = (ColType*)realloc((*query)->params.create_params.type_list, (i + 1) * sizeof(ColType));
        assert((*query)->params.create_params.type_list != NULL);

        (*query)->params.create_params.constraint_list = (ColConstraintType*)realloc((*query)->params.create_params.constraint_list, (i + 1) * sizeof(ColConstraintType));
        assert((*query)->params.create_params.constraint_list != NULL);

        // get "col_name"
        col_name = token;
        (*query)->params.create_params.col_list[i] = strdup(col_name);

        // get type
        token = strtok(NULL, " ,");
        if (!token){
            (*query)->cmd_type = INVALID;
            sprintf((*query)->syntax_message, "Syntax error: missing type for column '%s'.", col_name);
            return;
        }
        col_type = token;

        if (strcasecmp(col_type, "INT") == 0)
            (*query)->params.create_params.type_list[i] = INT;
        else if (strcasecmp(col_type, "STRING") == 0 || strcasecmp(col_type, "STR") == 0)
            (*query)->params.create_params.type_list[i] = STRING;
        else if (strcasecmp(col_type, "DOUBLE") == 0)
            (*query)->params.create_params.type_list[i] = DOUBLE;
        else {
            (*query)->cmd_type = INVALID;
            sprintf((*query)->syntax_message, "Invalid column type '%s' for column '%s'.", col_type, col_name);
            return;
        }

        // get pk/fk (optional)
        token = strtok(NULL, " ,");
        if (token && (strcasecmp(token, "PK") == 0 || strcasecmp(token, "FK") == 0)){
            col_constraint = token;
            (*query)->params.create_params.constraint_list[i] = (strcasecmp(col_constraint, "PK") == 0 ? PK : FK);
            token = strtok(NULL, " ,"); // move to next column
        } else {
            (*query)->params.create_params.constraint_list[i] = NONE;
        }

        (*query)->params.create_params.col_count++;
    }
}
