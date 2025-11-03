/*
Date of creation : 27/10/2025
Description : parse_delete to analyse delete command
Group 2 ESGI 2A3
*/

#include <string.h>
#include <stdio.h>

#include "parser.h"
#include "helper_ui.h"

void parse_delete(Query** query){
    char* token = NULL;
    char* extra_where_clause = NULL;
    char error_msg[200];

    (*query)->cmd_type = DELETE;

    // check FROM
    token = strtok(NULL, " \t");
    if(!contain_key_word(token, "FROM", query, "DELETE")) return;

    // check table name
    token = strtok(NULL, " \n");
    if(!contain_param(token, query, "1 table is required for DELETE statement")) return;
    strncpy((*query)->params.delete_params.table_name, token, sizeof((*query)->params.delete_params.table_name) - 1);

    // get WHERE (optional)
    extra_where_clause = strtok(NULL, "\n");

    if (extra_where_clause) {
        token = strtok(extra_where_clause, " \t");
        if(strcasecmp(token, "WHERE") == 0){
            // get col name
            token = strtok(NULL, " \t");
            if(!contain_param(token, query, "at least 1 column is required for WHERE clause")) return;
            strncpy((*query)->params.delete_params.condition_column, token, sizeof((*query)->params.delete_params.condition_column) - 1);
            
            // get "="
            token = strtok(NULL, " \t");
            if(!contain_key_word(token, "=", query, (*query)->params.delete_params.condition_column)) return;

            // get condition value
            token = strtok(NULL, " \t");
            sprintf(error_msg, "1 value is required for column '%s' in WHERE clause", (*query)->params.delete_params.condition_column);
            if(!contain_param(token, query, error_msg)) return; 
            strncpy((*query)->params.delete_params.condition_value, token, sizeof((*query)->params.delete_params.condition_value) - 1);

            token = strtok(NULL, "\n");
            check_end_of_cmd(token, query, "WHERE clause");
            return;
        } else{
            check_end_of_cmd(token, query, "DELETE statement");
            return;
        }
    }else return;
}
