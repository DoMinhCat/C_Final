/*
Date of creation : 27/10/2025
Description : parse_select to analyse select command
Group 2 ESGI 2A3
*/

#include <string.h>
#include <stdio.h>

#include "parser.h"
#include "helper_ui.h"

// todo : table list to drop
void parse_drop(Query** query){
    char* token;

    (*query)->cmd_type = DROP;

    // check TABLE
    token = strtok(NULL, " \t");
    if(!contain_key_word(token, "TABLE", query, "DROP")) return;

    // get table name to drop
    token = strtok(NULL, " \n");
    if(!contain_param(token, query, "at least 1 table is required for DROP statement")) return;
    strncpy((*query)->params.drop_params.table_name, token, sizeof((*query)->params.drop_params.table_name)-1);
}