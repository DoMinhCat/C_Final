/*
Date of creation : 17/10/2025
Description : Code for create_table function
Group 2 ESGI 2A3
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "db.h"
#include "helper.h"

Response* create_table(Query query){
    Response* res = init_response();
    Table* current_table = first_table;

    char* new_tb_name = query.params.create_params.table_name;
    int col_count = query.params.create_params.col_count;
    char** col_list = query.params.create_params.col_list;
    ColConstraintType* constraint_list = query.params.create_params.constraint_list;

    int i,j;

    // check table name
    while(current_table != NULL){
        if(strcmp(current_table->name, new_tb_name) == 0){
            res->status = FAILURE;
            sprintf(res->message, "Execution error : table '%s' already exist.", new_tb_name);
            return res;
        }
        current_table = current_table->next_table;
    }

    // check col names
    for(i=0; i<(col_count-1); i++){
        for(j=i+1; j<col_count; j++){
            if(strcmp(col_list[i], col_list[j]) == 0){
                res->status = FAILURE;
                sprintf(res->message, "Execution error : duplicated columns '%s' in table '%s'.", col_list[i], new_tb_name);
                return res;
            }
        }
    }

    //check 1 pk
    int pk_count = 0;
    for(i=0; i<sizeof(constraint_list)/sizeof(ColConstraintType); i++){
        if(constraint_list[i] == PK) pk_count++;
    }
    if(pk_count != 1){
        res->status = FAILURE;
        sprintf(res->message, "Execution error : a table must have a primary key column.");
        return res;
    }

    //check fk : reference to an existing table 
    int fk_count = query.params.create_params.fk_count;
    

    if(fk_count>0){
        int* fk_list_index = get_fk_col_list_index(query);
        assert(fk_list_index != NULL);
        int existing_refer_table_count = 0;
        int index_table_not_found;
        char** table_refer_list = query.params.create_params.table_refer_list;

        
        for(i=0; i<fk_count; i++){
            for(current_table = first_table; current_table != NULL; current_table = current_table->next_table){
                if(strcmp(current_table->name, table_refer_list[i]) == 0){
                    existing_refer_table_count++;
                }
            }
            if(existing_refer_table_count != i+1){
                index_table_not_found = i;
                break;
            }
        }
        if(existing_refer_table_count != fk_count){
            res->status = FAILURE;
            sprintf(res->message, "Execution error : table '%s' refered to by '%s' does not exist.", table_refer_list[index_table_not_found], col_list[fk_list_index[index_table_not_found]]);
            return res;
        }
    }

    
    while(current_table != NULL){
        if(strcmp(current_table->name, new_tb_name) == 0){
            res->status = FAILURE;
            sprintf(res->message, "Execution error : table '%s' already exist.", new_tb_name);
            return res;
        }
        current_table = current_table->next_table;
    }


    //check fk : + col of that table exists + that col is pk + col of same type + many cols refer to same col not allowed


    // create/malloc new table when all check is passed
    Table* new_tb = init_table();

    // set table name



}
/*
typedef struct 
{
    CommandType cmd_type; 

    // take one of these params based on cmd_type
    union{
        CreateParams create_params;
    } params;

    // set this if there is a syntax error that the parser detected
    char syntax_message[100];
} Query;

typedef struct{
    char table_name[TABLE_NAME_MAX];

    char **col_list; // list of column names passed in the query
    int col_count;          // number of columns

    ColType *type_list;  // list of types corresponding to column
    ColConstraintType *constraint_list; // list of constraint corresponding to order of col_list

    char** table_refer_list; //list of tables refered to by fk cols
    char** col_refer_list; // list of cols of refered tables refered to by fk cols
    int fk_count; // number of fk to free 2 lists above
} CreateParams;
*/