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

// todo: check where