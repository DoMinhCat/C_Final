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
        (*query)->cmd_type = INVALID;
        sprintf((*query)->syntax_message, "Syntax error: missing 'INTO' after INSERT.");
        return;
    }

    // get table name
    token = strtok(NULL, " \t");
    if(!token){
        (*query)->cmd_type = INVALID;
        sprintf((*query)->syntax_message, "Syntax error: missing table name after INTO.");
        return;
    }
    strncpy((*query)->params.insert_params.table_name, token, sizeof((*query)->params.insert_params.table_name)-1);
    (*query)->params.insert_params.table_name[sizeof((*query)->params.insert_params.table_name)-1] = '\0';
 
    // check '('
    token = strtok(NULL, " \t");
    if (!token || token[0] != '(') {
        (*query)->cmd_type = INVALID;
        sprintf((*query)->syntax_message, "Syntax error: missing '(' after table name.");
        return;
    }

    // get col_list : ( col1, col2 )
    char* col_list;
    char* value_keyword;
    char* open_value;
    char* data_list;
    int current_col_count;
    (*query)->params.insert_params.col_count = 0;

    col_list = strtok(NULL, ")"); // got " col1, col2, col3 " 
    value_keyword = strtok(NULL, " "); // got "VALUES"
    open_value = strtok(NULL, " \t"); // got "("
    data_list = strtok(NULL, ")"); // got "val1, val2 "
    if(!col_list){
        (*query)->cmd_type = INVALID;
        sprintf((*query)->syntax_message, "Syntax error: at least 1 column is required.");
        return;
    }

    // get each col by trimming "," and " "
    token = strtok(col_list, " ,\t"); // got col1
    while(token != NULL){
        current_col_count = (*query)->params.insert_params.col_count;

        // increase size of col_list 
        (*query)->params.insert_params.col_list = (char**)realloc((*query)->params.insert_params.col_list, (current_col_count+1) * sizeof(char*));
        assert(((*query)->params.insert_params.col_list) != NULL);
        // put into col_list
        (*query)->params.insert_params.col_list[current_col_count] = strdup(token);

        (*query)->params.insert_params.col_count++;
        // get next col
        token = strtok(NULL, ", ");
    }

    // check for "VALUES"
    if(!value_keyword || strcasecmp(value_keyword, "VALUES") != 0){
        (*query)->cmd_type = INVALID;
        sprintf((*query)->syntax_message, "Syntax error: missing 'VALUES' after column list.");
        return;
    }

    // check '('
    if (!open_value || open_value[0] != '(') {
        (*query)->cmd_type = INVALID;
        sprintf((*query)->syntax_message, "Syntax error: missing '(' after VALUES.");
        return;
    }

    // get data_list : ( val1, val2 )
    int val_count = 0;
    if(!data_list){
        (*query)->cmd_type = INVALID;
        sprintf((*query)->syntax_message, "Syntax error: at least 1 value is required.");
        return;
    }

    // get each col by trimming "," and " "
    token = strtok(data_list, " ,\t"); // got val1
    while(token != NULL){
        // increase size of data_list 
        (*query)->params.insert_params.data_list = (void**)realloc((*query)->params.insert_params.data_list, (val_count+1) * sizeof(void*)); 
        assert(((*query)->params.insert_params.data_list) != NULL);
        // set value
        (*query)->params.insert_params.data_list[val_count] = strdup(token);

        val_count++;
        // get next value
        token = strtok(NULL, ", ");
    }

    // check number of values passed and number of columns passed
    if(val_count != (*query)->params.insert_params.col_count){
        (*query)->cmd_type = INVALID;
        sprintf((*query)->syntax_message, "Syntax error: %d value(s) provided for %d column(s).", val_count, (*query)->params.insert_params.col_count);
        return;
    }
}