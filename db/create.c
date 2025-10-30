/*
Date of creation : 17/10/2025
Description : Code for create_table function
Group 2 ESGI 2A3
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include "db.h"
#include "helper.h"

Response* create_table(Query query){
    Response* res = init_response();
    Table* current_table = first_table;
    ColType* type_list = query.params.create_params.type_list;
    char* new_tb_name = query.params.create_params.table_name;
    int col_count = query.params.create_params.col_count;
    char** col_list = query.params.create_params.col_list;

    ColConstraintType* constraint_list = query.params.create_params.constraint_list;

    int i,j;

    // CHECK ALL INPUT 
    // check table name
    while(current_table){
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
    int pk_index = -1;
    int pk_count = 0;
    for(i=0; i<col_count; i++){
        if(constraint_list[i] == PK){ 
            pk_index = i;
            pk_count++;
            if(pk_count > 1){
                res->status = FAILURE;
                sprintf(res->message, "Execution error : a table must not have multiple primary key columns.");
                return res;
            }
        }
    }
    if(pk_index < 0){
        res->status = FAILURE;
        sprintf(res->message, "Execution error : a table must have a primary key column.");
        return res;
    }

    //check fk
    int fk_count = query.params.create_params.fk_count;

    if(fk_count>0){
        // refer to an existing table 
        int* fk_list_index = get_fk_col_list_index(query);
        assert(fk_list_index != NULL);
        bool refer_table_exist;
        bool refer_col_exist;
        char** table_refer_list = query.params.create_params.table_refer_list;
        char** col_refer_list = query.params.create_params.col_refer_list;
        Table* refered_table;
        Col* refered_col;
        Col* current_col;

        // refering to the table itself is not allowed, since it hasn't been created yet
        if (strcmp(new_tb_name, table_refer_list[i]) == 0) {
            res->status = FAILURE;
            sprintf(res->message, "Execution error : table '%s' cannot reference itself.", new_tb_name);
            return res;
        }
        
        // check many cols refer to same col not allowed
        for(i=0; i<fk_count-1; i++){
            for(j=i+1; j<fk_count; j++){
                if(strcmp(col_refer_list[i],col_refer_list[j]) == 0 && strcmp(table_refer_list[i], table_refer_list[j]) == 0){
                    res->status = FAILURE;
                    sprintf(res->message, "Execution error : many columns refering to a same column '%s' of table '%s' is not allowed.", col_refer_list[i], table_refer_list[i]);
                    return res;
                }
            }
        }

        // loop through all fk to check other criterias
        for(i=0; i<fk_count; i++){
            refer_table_exist = false;
            refer_col_exist = false;
            // check table refered exists
            //if it is the first table, it can't refer to anything
            if(!first_table){
                res->status = FAILURE;
                sprintf(res->message, "Execution error : table '%s' refered to by '%s' does not exist.", table_refer_list[i], col_list[fk_list_index[i]]);
                return res;
            }
            for(current_table = first_table; current_table != NULL; current_table = current_table->next_table){
                if(strcmp(current_table->name, table_refer_list[i]) == 0){
                    refer_table_exist = true; // flag to check if all fk refer to existing tables
                    refered_table = current_table; // get the pointer to table to check if refered col exists later
                    break;
                }
            }
            // if a table refered to doesn't exist, return error
            if(!refer_table_exist){
                res->status = FAILURE;
                sprintf(res->message, "Execution error : table '%s' refered to by '%s' does not exist.", table_refer_list[i], col_list[fk_list_index[i]]);
                return res;
            }

            // table exist, check for col refered to exists in that table
            for(current_col = refered_table->first_col; current_col != NULL; current_col = current_col->next_col){
                if(strcmp(current_col->name, col_refer_list[i]) == 0){
                    refer_col_exist = true; // flag to check if col exist
                    refered_col = current_col;
                    break;
                }
            }
            // if col doesn't exist, return error
            if(!refer_col_exist){
                res->status = FAILURE;
                sprintf(res->message, "Execution error : column '%s' does not exist in table '%s' refered to.", col_refer_list[i], table_refer_list[i]);
                return res;
            }

            // check if col is pk ?
            if(refered_col->constraint != PK){
                res->status = FAILURE;
                sprintf(res->message, "Execution error : column '%s' in table '%s' refered to is not a primary key.", col_refer_list[i], table_refer_list[i]);
                return res;
            }

            // check if col type is the same as col that refer to it
            if(refered_col->type != type_list[fk_list_index[i]]){
                res->status = FAILURE;
                sprintf(res->message, "Execution error : column '%s' in table '%s' refered to is not the same type as column '%s'.", col_refer_list[i], table_refer_list[i], col_list[fk_list_index[i]]);
                return res;
            }
        }
    }

    // create/malloc new table when all check is passed
    Table* new_tb = init_table();
    Col* new_col = NULL;

    // set table name
    new_tb->name = strdup(new_tb_name);

    // add col
    Col* current_col;
    // through through the col list and add them in the linked list
    for(i=0; i<col_count; i++){
        // set basic info
        new_col = init_col();
        new_col->name = strdup(col_list[i]);
        new_col->type = type_list[i];
        new_col->constraint = constraint_list[i];
        
        // set pointer to next col
        if(!new_tb->first_col){
            new_tb->first_col = new_col;
        } else{
            // get pointer to the last col in the list
            current_col = get_last_col(new_tb->first_col);
            // last col points to a new col
            current_col->next_col = new_col;
        }
    }

    // add table to the linked list
    if(!first_table) first_table = new_tb;
    else{
        current_table = get_last_table(first_table);
        current_table->next_table = new_tb;
    }
    table_count++;
    
    // init hash table
    HashTable* hash_table = init_hash_table();

    hash_table->pk_col_index = pk_index;
    hash_table->table_index = table_count-1;
    // add buckets as rows are inserted, there are 67 NULL buckets reserved

    // return success message
    res->status = SUCCESS;
    sprintf(res->message, "table '%s' created with %d column(s).", new_tb_name, col_count);
    return res;
}