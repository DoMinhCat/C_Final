/*
Date of creation : 27/10/2025
Description : parse_select to analyse select command
Group 2 ESGI 2A3
*/

#include <string.h>
#include <stdio.h>
#include "parser.h"

void parse_select(Query** query){
    char* token;
    char* col_list;
    char* col_name;
    char* from;
    char* table;
    char* after_table_name;
    int current_col_count = 0;

    (*query)->cmd_type = DELETE;

    // check "(" or *
    token = strtok(NULL, " \t"); // got ( or *
    if(!token || (strcmp(token, "(") != 0) && strcmp(token, "*") != 0){
        (*query)->cmd_type = INVALID;
        fprintf(stderr, "Syntax error: missing '(' or '*' after SELECT.");
        return;
    } 

    // case SELECT *
    if(strcmp(token, "*") == 0){
        (*query)->params.select_params.col_list = strdup(token);

        //check FROM
        token = strok(NULL, " \t");
        if(!token || strcasecmp(token, "FROM") != 0){
            (*query)->cmd_type = INVALID;
            fprintf(stderr, "Syntax error: missing 'FROM' after *.");
            return;
        }
        //check table name
        token = strok(NULL, " \t");
        if(!token || strlen(token) == 0){
            (*query)->cmd_type = INVALID;
            fprintf(stderr, "Syntax error: at least 1 table is required.");
            return;
        }
        //put table name in param
        strncpy((*query)->params.select_params.table_name, token, sizeof((*query)->params.create_params.table_name) - 1);
        
        after_table_name = strtok(NULL, " \t");
    } 
    // case SELECT ( col1, col2 )
    else if(strcmp(token, "(") == 0){
        //get col list " col1, col2, ... "
        col_list = strtok(NULL, ")");
        //get FROM 
        from = strtok(NULL, " \t");
        //get table name
        table = strtok(NULL, " \t");
        after_table_name = strtok(NULL, " \t");
        //get each col and put into col_list
        token = strtok(col_list, " ,\t"); // got col1
        while(token != NULL){
            current_col_count = (*query)->params.create_params.col_count;

            // increase size of col_list 
            (*query)->params.create_params.col_list = (char**)realloc((*query)->params.create_params.col_list, (current_col_count+1) * sizeof(char*));
            assert(((*query)->params.create_params.col_list) != NULL);
            // put into col_list
            (*query)->params.create_params.col_list[current_col_count] = strdup(token);

            (*query)->params.create_params.col_count++;
            // get next col
            token = strtok(NULL, ", ");
        }

        // check from
        if(!from || strcasecmp(from, "FROM") != 0){
            (*query)->cmd_type = INVALID;
            fprintf(stderr, "Syntax error: missing 'FROM' after ).");
            return;
        }
        // check table
        if(!table || strlen(token) == 0){
            (*query)->cmd_type = INVALID;
            fprintf(stderr, "Syntax error: at least 1 table is required.");
            return;
        }
        //put table name in param
        strncpy((*query)->params.select_params.table_name, token, sizeof((*query)->params.select_params.table_name) - 1);
    }

    // check optional where or join
    if(after_table_name && strlen(after_table_name) != 0){
        // case join
        if(strcasecmp(after_table_name, "JOIN") == 0){
            // check tab to join
            token = strtok(NULL, " \t");
            if(!token || strlen(token) == 0){
                (*query)->cmd_type = INVALID;
                fprintf(stderr, "Syntax error: at least 1 table is required.");
                return;
            }
            strncpy((*query)->params.select_params.table_join_name, token, sizeof((*query)->params.select_params.table_join_name) - 1);

            //check ON
            token = strtok(NULL, " \t");
            if(strcasecmp(token, "ON") != 0){
                (*query)->cmd_type = INVALID;
                fprintf(stderr, "Syntax error: missing 'ON' after table name.");
                return;
            }

            //check first col on
            token = strtok(NULL, " \t");
            if(!token || strlen(token) == 0){
                (*query)->cmd_type = INVALID;
                fprintf(stderr, "Syntax error: missing column name after ON.");
                return;
            }
            strncpy((*query)->params.select_params.first_col_on, token, sizeof((*query)->params.select_params.first_col_on) - 1);

            //check =
            token = strtok(NULL, " \t");
            if (!token || strlen(token) == 0) {
                (*query)->cmd_type = INVALID;
                sprintf((*query)->syntax_message, "Syntax error: missing '=' after '%s'.", (*query)->params.select_params.first_col_on);
                return;
            }
            if (strcmp(token, "=") != 0) {
                (*query)->cmd_type = INVALID;
                sprintf((*query)->syntax_message, "Syntax error: command '%s' not found, please check the syntax.", token);
                return;
            }

            //check second col on
            token = strtok(NULL, " \t");
            if(!token || strlen(token) == 0){
                (*query)->cmd_type = INVALID;
                fprintf(stderr, "Syntax error: missing column name after =.");
                return;
            }
            strncpy((*query)->params.select_params.second_col_on, token, sizeof((*query)->params.select_params.second_col_on) - 1);

            // check optional where after join

        }

        // case where
        else if(strcasecmp(after_table_name, "WHERE") == 0){
            
        } else{
            (*query)->cmd_type = INVALID;
            fprintf(stderr, "Syntax error: command '%s' not found, please check the syntax.", after_table_name);
            return;
        }
    }




}