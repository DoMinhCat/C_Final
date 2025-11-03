/*
Date of creation : 2/11/2025
Description : helper functions for parsing functions
Group 2 ESGI 2A3
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#include "helper_ui.h"
#include "parser.h"


bool contains_visible_char(char* str_to_check){
    // function to check at the end of commands if it is white spaces or invalid extra commands
    int i;

    if(!str_to_check) return false;

    for(i=0; str_to_check[i] != '\0'; i++){
        if(!isspace(str_to_check[i])) return true;
    }

    return false; // all is white spaces
}

void check_end_of_cmd(char* last_token, Query** query, char* current_stmt){
    if(contains_visible_char(last_token)){
        (*query)->cmd_type = INVALID;
        fprintf(stderr, "Syntax error: invalid command '%s' after '%s'.", last_token, current_stmt);
    }
}

bool contain_key_word(char* token, char* keyword, Query** query, char* current_stmt){
    if(!token){
        (*query)->cmd_type = INVALID;
        fprintf(stderr, "Syntax error: missing '%s' after '%s'.", keyword, current_stmt);
        return false;
    }
    if(strcasecmp(token, keyword) != 0){
        (*query)->cmd_type = INVALID;
        fprintf(stderr, "Syntax error: invalid command '%s' after '%s'.", token, current_stmt);
        return false;
    }
    return true;
}

bool contain_param(char* token, Query** query, char* err_msg){
    if(!token || strlen(token) == 0){
        (*query)->cmd_type = INVALID;
        fprintf(stderr, "Syntax error: %s.", err_msg);
        return false;
    }
    return true;
}

void check_where(char* token, Query** query){
    // check for the WHERE clause : WHERE x = y
    // when using this, need to check if cmd_type is INVALID at the end, if yes then return query

    char error_msg[200];
    char* condition_col = NULL;
    char* condition_val = NULL;

    //check condition column
    if(!contain_param(token, query, "at least 1 column is required for WHERE clause")) return;
    condition_col =  strdup(token);

    //check =
    token = strtok(NULL, " \t");
    if(!contain_key_word(token, "=", query, (*query)->params.select_params.condition_col)) return;

    //check condition value of where
    token = strtok(NULL, " \t");
    sprintf(error_msg, "1 value is required for column '%s' in WHERE clause", (*query)->params.select_params.condition_col);
    if(!contain_param(token, query, error_msg)) return; 
    condition_val =  strdup(token);
                
    // check for extra invalid command
    token = strtok(NULL, "\n");
    check_end_of_cmd(token, query, "WHERE clause");
    if((*query)->cmd_type == INVALID) return;

    // all check done, now assign to appropriate param
    switch ((*query)->cmd_type)
    {
    case SELECT:
        strcpy((*query)->params.select_params.condition_col, condition_col);
        strcpy((*query)->params.select_params.condition_val, condition_val);
        break;
    case DELETE:
        (*query)->params.delete_params.condition_column = strdup(condition_col);
        (*query)->params.delete_params.condition_value = strdup(condition_val);
        break;
    default:
        fprintf(stderr, "Syntax error: invalid use of WHERE clause.");
        break;
    }
}

// todo: check length of input
// todo : check identifier not containing banned chars/keyword