/*
Date of creation : 27/10/2025
Description : parse_create to analyse create command
Group 2 ESGI 2A3
*/

#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>

#include "parser.h"
#include "helper_ui.h"

void parse_create(Query** query){
    char* token;
    // get column names, type, pk/fk
    char* col_list = NULL;
    char* col_def = NULL;        // single column definition (e.g., "col int pk")
    char* tmp_col_def = NULL;
    char* col_name = NULL;
    char* col_type = NULL;
    char* col_constraint = NULL;
    char* reference_keyword = NULL;
    char* extra_cmd = NULL;
    int fk_count;
    char err_msg[200];

    char* list_to_free[] =  {token, col_list, col_def, tmp_col_def, col_name, col_type, col_constraint, reference_keyword, extra_cmd};
    int size = sizeof(list_to_free) / sizeof(list_to_free[0]);
    (*query)->cmd_type = CREATE;

    // check TABLE
    token = strtok(NULL, " \t");
    if(!contain_key_word(token, "TABLE", query, "CREATE")){
        free_create_parser(list_to_free, size);
        return;
    }

    // get table name to create
    token = strtok(NULL, " \t");
    if(!contain_param(token, query, "1 table name is required for CREATE statement")){
        free_create_parser(list_to_free, size);
        return;
    }

    //check max length
    if(exceed_max_len(token, query, TABLE_NAME_MAX, "table name")){
        free_create_parser(list_to_free, size);
        return;
    }

    // check reserved keyword and special chars
    if(!is_valid_identifier(token, query)){
        free_create_parser(list_to_free, size);
        return;
    }

    (*query)->params.create_params.table_name = strdup(token);

    // check '('
    token = strtok(NULL, " \t");
    if(!contain_key_word(token, "(", query, (*query)->params.create_params.table_name)){
        free_create_parser(list_to_free, size);
        return;
    }

    (*query)->params.create_params.fk_count = 0;
    (*query)->params.create_params.col_count = 0;
    

    // get col_list
    col_list = strtok(NULL, ")"); // got "col int pk, col int, col int"
    extra_cmd = strtok(NULL, "\n");
    if(!contain_param(col_list, query, "at least 1 column is required for CREATE statement")){
        free_create_parser(list_to_free, size);
        return;
    } 

    // get column definitions inside parentheses
    col_def = strtok(col_list, ","); // got "col int pk"

    while(col_def != NULL){
        fk_count = (*query)->params.create_params.fk_count;

        // parse on a tmp and keep the other to advance
        tmp_col_def = strdup(col_def);

        //check col name
        col_name = strtok(tmp_col_def, " \t");
        if(!contain_param(col_name, query, "at least 1 column is required for CREATE statement")){
            free_create_parser(list_to_free, size);
            return;
        } 
        
        // check len of col name
        if(exceed_max_len(col_name, query, TABLE_NAME_MAX, "column name")){
            free_create_parser(list_to_free, size);
            return;
        } 
        
        // check reserved keyword and special chars
        if(!is_valid_identifier(col_name, query)){
            free_create_parser(list_to_free, size);
            return;
        } 
        
        //set col name to col_list
        (*query)->params.create_params.col_list = (char**)realloc((*query)->params.create_params.col_list, ((*query)->params.create_params.col_count + 1) * sizeof(char*));
        assert((*query)->params.create_params.col_list != NULL);
        (*query)->params.create_params.col_list[(*query)->params.create_params.col_count] = strdup(col_name);
        (*query)->params.create_params.col_count++;

        //check col type
        col_type = strtok(NULL, " \t");
        sprintf(err_msg, "missing type for column '%s'", (*query)->params.create_params.col_list[(*query)->params.create_params.col_count - 1]);
        if(!contain_param(col_type, query, err_msg)){
            free_create_parser(list_to_free, size);
            return;
        } 
        //set col type
        (*query)->params.create_params.type_list = (ColType*)realloc((*query)->params.create_params.type_list, (*query)->params.create_params.col_count * sizeof(ColType));
        assert((*query)->params.create_params.type_list != NULL);

        if (strcasecmp(col_type, "INT") == 0)
            (*query)->params.create_params.type_list[(*query)->params.create_params.col_count - 1] = INT;
        else if (strcasecmp(col_type, "STRING") == 0 || strcasecmp(col_type, "STR") == 0)
            (*query)->params.create_params.type_list[(*query)->params.create_params.col_count - 1] = STRING;
        else if (strcasecmp(col_type, "DOUBLE") == 0)
            (*query)->params.create_params.type_list[(*query)->params.create_params.col_count - 1] = DOUBLE;
        else {
            (*query)->cmd_type = INVALID;
            fprintf(stderr, "Syntax error: invalid type '%s' for column '%s'.\n", col_type, (*query)->params.create_params.col_list[(*query)->params.create_params.col_count - 1]);
            free_create_parser(list_to_free, size);
            return;
        }

        // expand constraint list
        col_constraint = strtok(NULL, " \t");
        (*query)->params.create_params.constraint_list = (ColConstraintType*)realloc((*query)->params.create_params.constraint_list, (*query)->params.create_params.col_count * sizeof(ColConstraintType));
        assert((*query)->params.create_params.constraint_list != NULL);

        // check optional constraint
        if(col_constraint){
            if (strcasecmp(col_constraint, "PK") == 0){
                (*query)->params.create_params.constraint_list[(*query)->params.create_params.col_count - 1] = PK;
                
                // check for invalid extra cmd
                token =  strtok(NULL, " \t");
                if(token){
                    sprintf(err_msg, "constraint declaration of column '%s'", (*query)->params.create_params.col_list[(*query)->params.create_params.col_count - 1]);
                    check_end_of_cmd(token, query, err_msg);
                    free_create_parser(list_to_free, size);
                    return;
                } else{
                    free_create_parser(list_to_free, size);
                    return;
                }
            }
            else if (strcasecmp(col_constraint, "FK") == 0){
                (*query)->params.create_params.constraint_list[(*query)->params.create_params.col_count - 1] = FK;
                
                // check references
                token = strtok(NULL, " \t");
                if(!contain_key_word(token, "REFERENCES", query, "FK")){
                    free_create_parser(list_to_free, size);
                    return;
                }
                
                // check table refer
                token = strtok(NULL, " \t");
                if(!contain_param(token, query, "1 table is required for REFERENCES clause")){
                    free_create_parser(list_to_free, size);
                    return;
                }
                (*query)->params.create_params.table_refer_list = (char**)realloc((*query)->params.create_params.table_refer_list, (fk_count + 1) * sizeof(char*));
                assert(((*query)->params.create_params.table_refer_list) != NULL);
                
                (*query)->params.create_params.table_refer_list[fk_count] = strdup(token);
                
                // check col refer
                token = strtok(NULL, " \t");
                sprintf(err_msg, "1 column is required for table '%s' in REFERNCES clause", (*query)->params.create_params.table_refer_list[fk_count]);
                if(!contain_param(token, query, err_msg)){
                    free_create_parser(list_to_free, size);
                    return;
                }
                (*query)->params.create_params.col_refer_list = (char**)realloc((*query)->params.create_params.col_refer_list, (fk_count + 1) * sizeof(char*));
                assert(((*query)->params.create_params.col_refer_list) != NULL);
                
                (*query)->params.create_params.col_refer_list[fk_count] = strdup(token);
                (*query)->params.create_params.fk_count++;

                //check extra invalid cmd
                token = strtok(NULL, " \t");
                if(token){
                    check_end_of_cmd(token, query, "REFERENCES clause");
                    free_create_parser(list_to_free, size);
                    return;
                } else{
                    free_create_parser(list_to_free, size);
                    return;
                }
            }else{
                sprintf(err_msg, "column '%s'", (*query)->params.create_params.col_list[(*query)->params.create_params.col_count - 1]);
                check_end_of_cmd(col_constraint, query, err_msg);
                free_create_parser(list_to_free, size);
                return;
            }
            
        }else {
            (*query)->params.create_params.constraint_list[(*query)->params.create_params.col_count - 1] = NONE;
        }        

        //go to the next col def
        free(tmp_col_def);
        col_def = strtok(NULL, ",");
    }

    if(extra_cmd){
        check_end_of_cmd(extra_cmd, query, "CREATE statement");
        free_create_parser(list_to_free, size);
        return;
    } else{
        free_create_parser(list_to_free, size);
        return;
    }
}
