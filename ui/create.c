/*
Date of creation : 27/10/2025
Description : parse_create to analyse create command
Group 2 ESGI 2A3
*/

#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "parser.h"

void parse_create(Query** query){
    char* token;

    (*query)->cmd_type = CREATE;

    // check TABLE
    token = strtok(NULL, " ");
    if(!token || strcasecmp(token, "TABLE") != 0){
        (*query)->cmd_type = INVALID;
        sprintf((*query)->syntax_message, "Syntax error: missing 'TABLE' after CREATE.");
        return;
    }

    // get table name to create
    token = strtok(NULL, " ");
    if (!token) {
        (*query)->cmd_type = INVALID;
        sprintf((*query)->syntax_message, "Syntax error: missing table name after TABLE.");
        return;
    }
    strncpy((*query)->params.create_params.table_name, token, sizeof((*query)->params.create_params.table_name)-1);
    (*query)->params.create_params.table_name[sizeof((*query)->params.create_params.table_name)-1] = '\0';

    // check '('
    token = strtok(NULL, " ");
    if (!token || token[0] != '(') {
        (*query)->cmd_type = INVALID;
        sprintf((*query)->syntax_message, "Syntax error: missing '(' after table name.");
        return;
    }

    // get column names, type, pk/fk
    char* col_def;        // single column definition (e.g., "col int pk")
    char* col_name;
    char* col_type;
    char* col_constraint;
    char* reference_keyword;
    char* table_name_refer;
    char* col_name_refer;
    int i;

    (*query)->params.create_params.col_count = 0;

    // get first column definition inside parentheses
    col_def = strtok(NULL, ")");
    if (!col_def){
        (*query)->cmd_type = INVALID;
        sprintf((*query)->syntax_message, "Syntax error: at least 1 column is required.");
        return;
    }

    // split each column by ','
token = strtok(col_def, ",");
while(token != NULL){
    // Trim spaces
    while(*token == ' ' || *token == '\t') token++;
    char* end = token + strlen(token) - 1;
    while(end > token && (*end == ' ' || *end == '\t')) { *end = '\0'; end--; }
    
    col_name = token;
    
    // Find first space/tab to separate name from type
    col_type = NULL;
    col_constraint = NULL;
    reference_keyword = NULL;
    table_name_refer = NULL;
    col_name_refer = NULL
    
    char* p = token;
    while(*p && *p != ' ' && *p != '\t') p++;
    
    if(*p) {
        *p = '\0'; 
        p++;
        while(*p == ' ' || *p == '\t') p++;  // Skip spaces
        
        if(*p) {
            col_type = p;
            // Find next space for constraint
            while(*p && *p != ' ' && *p != '\t') p++;
            
            if(*p) {
                *p = '\0';  // Terminate col_type
                p++;
                while(*p == ' ' || *p == '\t') p++;  // Skip spaces
                
                if(*p) {
                    col_constraint = p;
                    // Find next space for REFERENCES
                    while(*p && *p != ' ' && *p != '\t') p++;

                    if(*p){
                        *p = '\0';  // Terminate constraint
                        p++;  
                        while(*p == ' ' || *p == '\t') p++;  // Skip spaces

                        if(*p){
                            reference_keyword = p;
                            // Find next space for refer table name
                            while(*p && *p != ' ' && *p != '\t') p++;

                            if(*p){
                                *p = '\0';  // Terminate REFERENCES
                                p++;  
                                while(*p == ' ' || *p == '\t') p++;  // Skip spaces

                                if(*p){
                                    table_name_refer = p;
                                    // Find next space for refer col name
                                    while(*p && *p != ' ' && *p != '\t') p++;

                                    if(*p){
                                        *p = '\0';  // Terminate refer table name
                                        p++;  
                                        while(*p == ' ' || *p == '\t') p++;  // Skip spaces

                                        if(*p){
                                            col_name_refer = p;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    // Validate
    if (!col_name || strlen(col_name) == 0){
        (*query)->cmd_type = INVALID;
        sprintf((*query)->syntax_message, "Syntax error: at least 1 column is required.");
        return;
    }

    if (!col_type){
        (*query)->cmd_type = INVALID;
        sprintf((*query)->syntax_message, "Syntax error: missing type for column '%s'.", col_name);
        return;
    }

    // Allocate and fill (rest of your code stays the same)
    i = (*query)->params.create_params.col_count;

    (*query)->params.create_params.col_list = (char**)realloc((*query)->params.create_params.col_list, (i + 1) * sizeof(char*));
    (*query)->params.create_params.type_list = (ColType*)realloc((*query)->params.create_params.type_list, (i + 1) * sizeof(ColType));
    (*query)->params.create_params.constraint_list = (ColConstraintType*)realloc((*query)->params.create_params.constraint_list, (i + 1) * sizeof(ColConstraintType));
    assert((*query)->params.create_params.col_list != NULL);
    assert((*query)->params.create_params.type_list != NULL);
    assert((*query)->params.create_params.constraint_list != NULL);

    (*query)->params.create_params.col_list[i] = strdup(col_name);

    if (strcasecmp(col_type, "INT") == 0)
        (*query)->params.create_params.type_list[i] = INT;
    else if (strcasecmp(col_type, "STRING") == 0 || strcasecmp(col_type, "STR") == 0)
        (*query)->params.create_params.type_list[i] = STRING;
    else if (strcasecmp(col_type, "DOUBLE") == 0)
        (*query)->params.create_params.type_list[i] = DOUBLE;
    else {
        (*query)->cmd_type = INVALID;
        sprintf((*query)->syntax_message, "Invalid column type '%s' for column '%s'.", col_type, col_name);
        return;
    }

    if (col_constraint){
        if (strcasecmp(col_constraint, "PK") == 0)
            (*query)->params.create_params.constraint_list[i] = PK;
        else if (strcasecmp(col_constraint, "FK") == 0)
            (*query)->params.create_params.constraint_list[i] = FK;

            if (!reference_keyword){
                (*query)->cmd_type = INVALID;
                sprintf((*query)->syntax_message, "Syntax error: missing 'REFERENCES' after FK.");
                return;
            }
            if (!table_name_refer){
                (*query)->cmd_type = INVALID;
                sprintf((*query)->syntax_message, "Syntax error: missing table name to refer to after REFERENCES.");
                return;
            }
            if (!col_name_refer){
                (*query)->cmd_type = INVALID;
                sprintf((*query)->syntax_message, "Syntax error: missing column name to refer to after table name.");
                return;
            }

        else {
            (*query)->cmd_type = INVALID;
            sprintf((*query)->syntax_message, "Invalid constraint '%s' for column '%s'.", col_constraint, col_name);
            return;
        }
    } else (*query)->params.create_params.constraint_list[i] = NONE;

    (*query)->params.create_params.col_count++;

    // next column
    token = strtok(NULL, ",");
    }
}
