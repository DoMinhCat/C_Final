/*
Date of creation : 27/10/2025
Description : parse_insert to analyse insert command
Group 2 ESGI 2A3
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "parser.h"

void parse_insert(Query** query){
    char* token;

    (*query)->cmd_type = INSERT;

    // check "INTO"
    token = strtok(NULL, " \t");
    if(!token || strcasecmp(token, "INTO") != 0){
        (*query).cmd_type = INVALID;
        sprintf((*query).syntax_message, "Syntax error: missing 'INTO' after INSERT.");
        return;
    }

    // get table name
    token = strtok(NULL, " \t");
    if(!token){
        (*query).cmd_type = INVALID;
        sprintf((*query).syntax_message, "Syntax error: missing table name after INTO.");
        return;
    }
    strncpy((*query).params.insert_params.table_name, token, sizeof((*query).params.insert_params.table_name)-1);
    (*query).params.insert_params.table_name[sizeof((*query).params.insert_params.table_name)-1] = "\0";
 
    // check '('
    token = strtok(NULL, " \t");
    if (!token || token[0] != '(') {
        (*query)->cmd_type = INVALID;
        sprintf((*query)->syntax_message, "Syntax error: missing '(' after table name.");
        return;
    }

    // get col_list : ( col1, col2 )
    char* col_list;
    int i;
    (*query)->params.insert_params.col_count = 0;

    col_list = strtok(NULL, ")"); // got " col1, col2, col3 "
    if(!col_list){
        (*query)->cmd_type = INVALID;
        sprintf((*query)->syntax_message, "Syntax error: at least 1 column is required.");
        return;
    }

    // get each col by trimming "," and " "
    token = strtok(col_list, " ,\t"); // got col1
    while(token != NULL){
        i = (*query)->params.insert_params.col_count;

        // increase size of col_list 
        (*query)->params.insert_params.col_list = (char**)realloc((*query)->params.insert_params.col_list, (i+1) * sizeof(char*));
        assert(((*query)->params.insert_params.col_list) != NULL);
        // set col name
        (*query)->params.create_params.col_list[i] = strdup(col_name);

        (*query)->params.insert_params.col_count++;
        // get next col
        token = strtok(NULL, ",");
    }

    // check for "VALUES"
    token = strtok(NULL, " ");
    if(!token || strcasecmp(token, "VALUES")){
        (*query)->cmd_type = INVALID;
        sprintf((*query)->syntax_message, "Syntax error: missing 'VALUES' after column list.");
        return;
    }
    //insert into tab (col1, col2) values ( 1, sds, sss)

}