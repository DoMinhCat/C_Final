/*
Date of creation : 27/10/2025
Description : parse_select to analyse select command
Group 2 ESGI 2A3
*/
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "parser.h"
#include "helper_ui.h"

void parse_select(Query** query){
    char* token = NULL;
    char* col_list = NULL;
    char* col_name = NULL;
    char* from = NULL;
    char* table = NULL;
    char* after_table_name = NULL;
    char* extra_clause = NULL;
    char* extra_of_join = NULL;
    char* extra_of_where = NULL;
    int current_col_count = 0;

    (*query)->cmd_type = SELECT;
    (*query)->params.select_params.col_count = 0;

    // check "(" or *
    token = strtok(NULL, " \t"); // got ( or *
    if(!token || (strcmp(token, "(") != 0) && strcmp(token, "*") != 0){
        (*query)->cmd_type = INVALID;
        fprintf(stderr, "Syntax error: missing '(' or '*' after SELECT.");
        return;
    } 

    // case SELECT *
    if(strcmp(token, "*") == 0){
        (*query)->params.select_params.col_list = (char**)malloc(sizeof(char*));
        assert((*query)->params.select_params.col_list != NULL);
        // no need memory allocation thanks to strdup :))
        (*query)->params.select_params.col_list[0] = strdup(token);
        (*query)->params.select_params.col_count++;

        //check FROM
        token = strtok(NULL, " \t");
        if(!contain_key_word(token, "FROM", query, "*")) return;
        //check table name
        token = strtok(NULL, " \t");
        if(!contain_param(token, query, "at least 1 table is required")) return;
        //put table name in param
        strncpy((*query)->params.select_params.table_name, token, sizeof((*query)->params.select_params.table_name) - 1);
        
        extra_clause = strtok(NULL, "\n");
    } 
    // case SELECT ( col1, col2 )
    else if(strcmp(token, "(") == 0){
        //get col list " col1, col2, ... "
        col_list = strtok(NULL, ")");
        //get FROM 
        from = strtok(NULL, " \t");
        //get table name
        table = strtok(NULL, " \t");
        //check end of command
        extra_clause = strtok(NULL, "\n");

        //get each col and put into col_list
        token = strtok(col_list, " ,\t"); // got col1
        while(token != NULL){
            current_col_count = (*query)->params.select_params.col_count;

            // increase size of col_list 
            (*query)->params.select_params.col_list = (char**)realloc((*query)->params.select_params.col_list, (current_col_count+1) * sizeof(char*));
            assert(((*query)->params.select_params.col_list) != NULL);
            // put into col_list
            (*query)->params.select_params.col_list[current_col_count] = strdup(token);

            (*query)->params.select_params.col_count++;
            // get next col
            token = strtok(NULL, " ,\t");
        }

        // check from
        if(!from || strcasecmp(from, "FROM") != 0){
            (*query)->cmd_type = INVALID;
            fprintf(stderr, "Syntax error: missing 'FROM' after ).");
            return;
        }
        // check table
        if(!table || strlen(table) == 0){
            (*query)->cmd_type = INVALID;
            fprintf(stderr, "Syntax error: at least 1 table is required.");
            return;
        }
        //put table name in param
        strncpy((*query)->params.select_params.table_name, table, sizeof((*query)->params.select_params.table_name) - 1);
    }

    // no more optional where or join
    if(!extra_clause) return;
    // check optional where or join
    else {
        // case join
        token = strtok(extra_clause, " \t");
        if(strcasecmp(token, "JOIN") == 0){
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
                fprintf(stderr, "Syntax error: missing '=' after '%s'.", (*query)->params.select_params.first_col_on);
                return;
            }
            if (strcmp(token, "=") != 0) {
                (*query)->cmd_type = INVALID;
                fprintf(stderr, "Syntax error: command '%s' not found, please check the syntax.", token);
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
            extra_of_join = strtok(NULL, "\n");
            if(extra_of_join){
                token = strtok(extra_of_join, " \t");
                if(strcasecmp(token, "WHERE") == 0){
                    //check condition column
                    token = strtok(NULL, " \t");
                    if(!token || strlen(token) == 0){
                        (*query)->cmd_type = INVALID;
                        fprintf(stderr, "Syntax error: at least 1 column is required after WHERE.");
                        return;
                    }
                    strncpy((*query)->params.select_params.condition_col, token, sizeof((*query)->params.select_params.condition_col) - 1);

                    //check =
                    token = strtok(NULL, " \t");
                    if (!token || strlen(token) == 0) {
                        (*query)->cmd_type = INVALID;
                        fprintf(stderr, "Syntax error: missing '=' after '%s'.", (*query)->params.select_params.condition_col);
                        return;
                    }
                    if (strcmp(token, "=") != 0) {
                        (*query)->cmd_type = INVALID;
                        fprintf(stderr, "Syntax error: command '%s' not found, please check the syntax.", token);
                        return;
                    }

                    //check condition value of where
                    token = strtok(NULL, " \t");
                    if (!token || strlen(token) == 0) {
                        (*query)->cmd_type = INVALID;
                        fprintf(stderr, "Syntax error: missing value for column '%s' in WHERE clause.", (*query)->params.select_params.condition_col);
                        return;
                    }
                    strncpy((*query)->params.select_params.condition_val, token, sizeof((*query)->params.select_params.condition_val) - 1);
                    
                    token = strtok(NULL, "\n");
                    check_end_of_cmd(token, query, "WHERE clause");
                    return;
                }else{
                    check_end_of_cmd(token, query, "JOIN clause");
                    return;
                }
            } 
            // no extra clause after join
            else return; 
        }

        // case where
        else if(strcasecmp(token, "WHERE") == 0){
            //check condition column
            token = strtok(NULL, " \t");
            if(!token || strlen(token) == 0){
                (*query)->cmd_type = INVALID;
                fprintf(stderr, "Syntax error: at least 1 column is required after WHERE.");
                return;
            }
            strncpy((*query)->params.select_params.condition_col, token, sizeof((*query)->params.select_params.condition_col) - 1);

            //check =
            token = strtok(NULL, " \t");
            if (!token || strlen(token) == 0) {
                (*query)->cmd_type = INVALID;
                fprintf(stderr, "Syntax error: missing '=' after '%s'.", (*query)->params.select_params.condition_col);
                return;
            }
            if (strcmp(token, "=") != 0) {
                (*query)->cmd_type = INVALID;
                fprintf(stderr, "Syntax error: command '%s' not found, please check the syntax.", token);
                return;
            }

            //check condition value of where
            token = strtok(NULL, " \t");
            if (!token || strlen(token) == 0) {
                (*query)->cmd_type = INVALID;
                fprintf(stderr, "Syntax error: missing value in WHERE clause.");
                return;
            }
            strncpy((*query)->params.select_params.condition_val, token, sizeof((*query)->params.select_params.condition_val) - 1);
                        
            // check for extra invalid command
            extra_of_where = strtok(NULL, "\n");
            if(extra_of_where){
                (*query)->cmd_type = INVALID;
                fprintf(stderr, "Syntax error: invalid command '%s' after WHERE clause.", extra_of_where);
                return;
            }

            
        } else{
            (*query)->cmd_type = INVALID;
            fprintf(stderr, "Syntax error: invalid command '%s' after SELECT statement.", extra_clause);
            return;
        }
    } 
}