/*
Date of creation : 27/10/2025
Description : parse_select to analyse select command
Group 2 ESGI 2A3
*/

#include <string.h>
#include <stdio.h>
#include "parser.h"

void parse_drop(Query** query){
    char* token;

    (*query)->cmd_type = DROP;

    // check TABLE
    token = strtok(NULL, " ");
    if(!token || strcasecmp(token, "TABLE") != 0){
        (*query)->cmd_type = INVALID;
        sprintf((*query)->syntax_message, "Syntax error: missing 'TABLE' after DROP.");
        return;
    }

    // get table name to drop
    token = strtok(NULL, " \n");
    if (!token) {
        (*query)->cmd_type = INVALID;
        sprintf((*query)->syntax_message, "Syntax error: missing table name after TABLE.");
        return;
    }
    strncpy((*query)->params.drop_params.table_name, token, sizeof((*query)->params.drop_params.table_name)-1);
}