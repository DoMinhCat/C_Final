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
#include "helper_ui.h"

void parse_insert(Query** query){
    char* token;

    (*query)->cmd_type = INSERT;

    // check "INTO"
    token = strtok(NULL, " \t");
    if(!contain_key_word(token, "INTO", query, "INSERT statement")) return;

    // get table name
    token = strtok(NULL, " \t");
    if(!contain_param(token, query, "1 table is required for INSERT statement")) return;
    (*query)->params.insert_params.table_name = strdup(token);
 
    // check '('
    token = strtok(NULL, " \t");
    if(!contain_key_word(token, ")", query, (*query)->params.insert_params.table_name)) return;

    // get col_list : ( col1, col2 )
    char* col_list;
    char* value_keyword;
    char* open_value;
    char* data_list;
    int current_col_count;
    (*query)->params.insert_params.col_count = 0;

    col_list = strtok(NULL, ")"); // got " col1, col2, col3 " 
    value_keyword = strtok(NULL, " \t"); // got "VALUES"
    open_value = strtok(NULL, " \t"); // got "("
    data_list = strtok(NULL, ")"); // got "val1, val2 "

    if(!contain_param(col_list, query, "at least 1 column is required for INSERT statement")) return;

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
        token = strtok(NULL, "\t, ");
    }

    // check for "VALUES"
    if(!contain_key_word(value_keyword, "VALUE", query, "column list in INSERT statement")) return;

    // check '('
    if(!contain_key_word(open_value, "(", query, "VALUES")) return;

    // get data_list : ( val1, val2 )
    int val_count = 0;
    if(!contain_param(data_list, query, "at least 1 value is required for INSERT statement")) return;

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
        token = strtok(NULL, "\t, ");
    }

    // check number of values passed and number of columns passed
    if(val_count != (*query)->params.insert_params.col_count){
        (*query)->cmd_type = INVALID;
        fprintf(stderr, "Syntax error: %d value(s) provided for %d column(s).", val_count, (*query)->params.insert_params.col_count);
        return;
    }
}