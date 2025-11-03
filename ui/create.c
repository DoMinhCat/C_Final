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
    
    int i;
    (*query)->cmd_type = CREATE;

    // check TABLE
    token = strtok(NULL, " \t");
    if(!contain_key_word(token, "TABLE", query, "CREATE")) return;

    // get table name to create
    token = strtok(NULL, " \t");
    if(!contain_param(token, query, "1 table name is required for CREATE statement")) return;

    //check max length
    if(exceed_max_len(token, query, TABLE_NAME_MAX, "table name")) return;

    // check reserved keyword and special chars
    if(!is_valid_identifier(token, query)) return;

    (*query)->params.create_params.table_name = strdup(token);

    // check '('
    token = strtok(NULL, " \t");
    if(!contain_key_word(token, "(", query, (*query)->params.create_params.table_name)) return;

    // get column names, type, pk/fk
    char* col_list = NULL;
    char* col_def = NULL;        // single column definition (e.g., "col int pk")
    char* col_name = NULL;
    char* col_type = NULL;
    char* col_constraint = NULL;
    char* reference_keyword = NULL;
    char* table_name_refer = NULL;
    char* col_name_refer = NULL;
    int fk_count;
    int col_count;
    char err_msg[200];

    (*query)->params.create_params.fk_count = 0;
    (*query)->params.create_params.col_count = 0;
    

    // get col_list
    col_list = strtok(NULL, ")");
    if(!contain_param(col_list, query, "at least 1 column is required for CREATE statement")) return;

    // get column definitions inside parentheses
    col_def = strtok(col_list, ",");
    while(col_def != NULL){
        col_count = (*query)->params.create_params.col_count;
        fk_count = (*query)->params.create_params.fk_count;

        //check col name
        col_name = strtok(col_def, " \t");
        if(!contain_param(col_name, query, "at least 1 column is required for CREATE statement")) return;
        
        // check len of col name
        if(exceed_max_len(col_name, query, TABLE_NAME_MAX, "column name")) return;
        
        // check reserved keyword and special chars
        if(!is_valid_identifier(col_name, query)) return;
        
        //set col name to col_list
        (*query)->params.create_params.col_list = (char**)realloc((*query)->params.create_params.col_list, (col_count + 1) * sizeof(char*));
        assert((*query)->params.create_params.col_list != NULL);
        (*query)->params.create_params.col_list[col_count] = strdup(col_name);

        //check col type
        col_type = strtok(NULL, " \t");
        if(!contain_param(token, query, err_msg)) return;
        //set col type
        (*query)->params.create_params.type_list = (ColType*)realloc((*query)->params.create_params.type_list, (col_count + 1) * sizeof(ColType));
        assert((*query)->params.create_params.type_list != NULL);

        if (strcasecmp(col_type, "INT") == 0)
            (*query)->params.create_params.type_list[col_count] = INT;
        else if (strcasecmp(col_type, "STRING") == 0 || strcasecmp(col_type, "STR") == 0)
            (*query)->params.create_params.type_list[col_count] = STRING;
        else if (strcasecmp(col_type, "DOUBLE") == 0)
            (*query)->params.create_params.type_list[col_count] = DOUBLE;
        else {
            (*query)->cmd_type = INVALID;
            fprintf(stderr, "Syntax error: invalid type '%s' for column '%s'.", (*query)->params.create_params.type_list[col_count], (*query)->params.create_params.col_list[col_count]);
            return;
        }

        // check optional constraint
        col_constraint = strtok(NULL, " \t");
        if(col_constraint){
            // expand constraint list
            (*query)->params.create_params.constraint_list = (ColConstraintType*)realloc((*query)->params.create_params.constraint_list, (fk_count + 1) * sizeof(ColConstraintType));
            assert((*query)->params.create_params.constraint_list != NULL);

            if (strcasecmp(col_constraint, "PK") == 0){
                (*query)->params.create_params.constraint_list[fk_count] = PK;
                (*query)->params.create_params.fk_count++;
                
                // check for invalid extra cmd
                token =  strtok(NULL, " \t");
                sprintf(err_msg, "constraint declaration of column '%s'", (*query)->params.create_params.col_list[col_count]);
                check_end_of_cmd(token, query, err_msg);
                return;
            }
            else if (strcasecmp(col_constraint, "FK") == 0){
                (*query)->params.create_params.constraint_list[fk_count] = FK;
                
                // check references
                token = strtok(NULL, " \t");
                if(!contain_key_word(token, "REFERENCES", query, "FK")) return;
                
                // check table refer
                token = strtok(NULL, " \t");
                if(!contain_param(token, query, "1 table is required for REFERENCES clause")) return;
                (*query)->params.create_params.table_refer_list = (char**)realloc((*query)->params.create_params.table_refer_list, (fk_count + 1) * sizeof(char*));
                assert(((*query)->params.create_params.table_refer_list) != NULL);
                
                (*query)->params.create_params.table_refer_list[fk_count] = strdup(table_name_refer);
                
                // check col refer
                token = strtok(NULL, " \t");
                sprintf(err_msg, "1 column is required for table '%s' in REFERNCES clause", (*query)->params.create_params.table_refer_list[fk_count]);
                if(!contain_param(token, query, err_msg)) return;
                (*query)->params.create_params.col_refer_list = (char**)realloc((*query)->params.create_params.col_refer_list, (fk_count + 1) * sizeof(char*));
                assert(((*query)->params.create_params.col_refer_list) != NULL);
                
                (*query)->params.create_params.col_refer_list[fk_count] = strdup(col_name_refer);
                (*query)->params.create_params.fk_count++;

                //check extra invalid cmd
                token = strtok(NULL, " \t");
                check_end_of_cmd(token, query, "REFERENCES clause");
                return;
            }else{
                sprintf(err_msg, "type declaration of column '%s'", (*query)->params.create_params.col_list[col_count]);
                check_end_of_cmd(col_constraint, query, err_msg);
                return;
            }
            
        }else return;        
        (*query)->params.create_params.col_count++;

        //go to the next col def
        col_def = strtok(col_list, ",");
        for(i=0; i<col_count; i++){
            col_def = strtok(NULL, ",");
        }
    }
}
