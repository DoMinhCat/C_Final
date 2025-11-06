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
#include "helper_db.h"
#include "../main.h"
#include "../ui/parser.h"
#include "../init/init.h"
#include "../hash/hash.h"
#include "../global_var.h"
#include "../clean/clean.h"


void create_table(Query* query){
    Table* current_table = first_table;

    ColType* type_list = query->params.create_params.type_list;
    char* new_tb_name = query->params.create_params.table_name;
    int col_count = query->params.create_params.col_count;
    char** col_list = query->params.create_params.col_list;
    ColConstraintType* constraint_list = query->params.create_params.constraint_list;
    char** refer_table_list =  query->params.create_params.table_refer_list;
    char** refer_col_list =  query->params.create_params.col_refer_list;

    int i,j,k;

    // check table name
    while(current_table){
        if(strcmp(current_table->name, new_tb_name) == 0){
            fprintf(stderr, "Execution error: table '%s' already exist.\n", new_tb_name);
            return;
        }
        current_table = current_table->next_table;
    }

    // check col names
    for(i=0; i<(col_count-1); i++){
        for(j=i+1; j<col_count; j++){
            if(strcmp(col_list[i], col_list[j]) == 0){
                fprintf(stderr, "Execution error: duplicated columns '%s' in table '%s'.\n", col_list[i], new_tb_name);
                return;
            }
        }
    }

    //check 1 pk
    int pk_count = 0;
    char* pk_col_name = NULL; // to set in hash table later
    for(i=0; i<col_count; i++){
        if(constraint_list[i] == PK){ 
            // free before strdup in case 2 pk
            free(pk_col_name);
            pk_col_name = NULL;

            pk_col_name = strdup(col_list[i]);
            assert(pk_col_name != NULL);

            pk_count++;
            if(pk_count > 1){
                fprintf(stderr, "Execution error: a table must not have multiple primary key columns.\n");
                free(pk_col_name);
                pk_col_name = NULL;
                return;
            }
        }
    }
    if(pk_count != 1){
        fprintf(stderr, "Execution error: a table must have a primary key column.\n");
        free(pk_col_name);
        pk_col_name = NULL;
        return;
    }

    //check fk
    int fk_count = query->params.create_params.fk_count;

    if(fk_count>0){
        // refer to an existing table 
        int* fk_list_index = get_fk_col_list_index(query);
        assert(fk_list_index != NULL);
        bool refer_table_exist;
        bool refer_col_exist;
        char** table_refer_list = query->params.create_params.table_refer_list;
        char** col_refer_list = query->params.create_params.col_refer_list;
        Table* refered_table = NULL;
        Col* refered_col = NULL;
        Col* current_col = NULL;

        // refering to the table itself is not allowed, since it hasn't been created yet
        for(j=0; j<fk_count; j++){
            if (strcmp(new_tb_name, table_refer_list[j]) == 0) {
                fprintf(stderr, "Execution error: table '%s' cannot reference itself.\n", new_tb_name);
                free(pk_col_name);
                pk_col_name = NULL;
                return;
            }
        }
        
        // check many cols refer to same col not allowed
        for(j=0; j<fk_count-1; j++){
            for(k=j+1; k<fk_count; k++){
                if(strcmp(col_refer_list[j],col_refer_list[k]) == 0 && strcmp(table_refer_list[j], table_refer_list[k]) == 0){
                    fprintf(stderr, "Execution error: many columns refering to a same column '%s' of table '%s' is not allowed.\n", col_refer_list[j], table_refer_list[j]);
                    free(pk_col_name);
                    pk_col_name = NULL;
                    return;
                }
            }
        }

        // loop through all fk to check other criterias
        for(j=0; j<fk_count; j++){
            refer_table_exist = false;
            refer_col_exist = false;
            // check table refered exists
            //if it is the first table, it can't refer to anything
            if(!first_table){
                fprintf(stderr, "Execution error: table '%s' refered to by '%s' does not exist.\n", table_refer_list[j], col_list[fk_list_index[j]]);
                free(pk_col_name);
                pk_col_name = NULL;
                return;
            }
            for(current_table = first_table; current_table != NULL; current_table = current_table->next_table){
                if(strcmp(current_table->name, table_refer_list[j]) == 0){
                    refer_table_exist = true; // flag to check if all fk refer to existing tables
                    refered_table = current_table; // get the pointer to table to check if refered col exists later
                    break;
                }
            }
            // if a table refered to doesn't exist, return error
            if(!refer_table_exist){
                fprintf(stderr, "Execution error: table '%s' refered to by '%s' does not exist.\n", table_refer_list[j], col_list[fk_list_index[j]]);
                free(pk_col_name);
                pk_col_name = NULL;
                return;
            }

            // table exist, check for col refered to exists in that table
            for(current_col = refered_table->first_col; current_col != NULL; current_col = current_col->next_col){
                if(strcmp(current_col->name, col_refer_list[j]) == 0){
                    refer_col_exist = true; // flag to check if col exist
                    refered_col = current_col;
                    break;
                }
            }
            // if col doesn't exist, return error
            if(!refer_col_exist){
                fprintf(stderr, "Execution error: column '%s' does not exist in table '%s' refered to.\n", col_refer_list[j], table_refer_list[j]);
                free(pk_col_name);
                pk_col_name = NULL;
                return;
            }

            // check if col is pk ?
            if(refered_col->constraint != PK){
                fprintf(stderr, "Execution error: column '%s' in table '%s' refered to is not a primary key.\n", col_refer_list[j], table_refer_list[j]);
                free(pk_col_name);
                pk_col_name = NULL;
                return;
            }

            // check if col type is the same as col that refer to it
            if(refered_col->type != type_list[fk_list_index[j]]){
                fprintf(stderr, "Execution error: column '%s' in table '%s' refered to is not the same type as column '%s'.\n", col_refer_list[j], table_refer_list[j], col_list[fk_list_index[j]]);
                free(pk_col_name);
                pk_col_name = NULL;
                return;
            }
        }
    }

    // create/malloc new table when all check is passed
    Table* new_tb = init_table();
    Col* new_col = NULL;

    // set table name
    new_tb->name = strdup(new_tb_name);
    assert(new_tb->name != NULL);

    // add col
    Col* current_col;
    int refer_list_index=0;
    // loop through the col list and add them in the linked list
    for(i=0; i<col_count; i++){
        // set basic info
        new_col = init_col();
        new_col->name = strdup(col_list[i]);
        assert(new_col->name != NULL);
        new_col->type = type_list[i];
        new_col->constraint = constraint_list[i];

        // TODO : if col to add is fk, set table and col it refers to
        if(new_col->constraint == FK){
            new_col->refer_table = strdup(refer_table_list[refer_list_index]);
            assert(new_col->refer_table!=NULL);

            new_col->refer_col = strdup(refer_col_list[refer_list_index]);
            assert(new_col->refer_col!=NULL);

            refer_list_index++;
        }
        // set pointer to next col
        if (new_tb->first_col == NULL || new_tb->first_col->name == NULL) {
            // free the dummy if it exists
            if (new_tb->first_col && new_tb->first_col->name == NULL)
                free_col(new_tb->first_col);
            new_tb->first_col = new_col;
        } else {
            // append to the end
            current_col = get_last_col(new_tb->first_col);
            current_col->next_col = new_col;
        }
    }
    new_tb->col_count = col_count;

    // add table to the linked list
    if(!first_table) first_table = new_tb;
    else{
        current_table = get_last_table(first_table);
        current_table->next_table = new_tb;
    }
    
    // init hash table
    HashTable* hash_table = init_hash_table();

    hash_table->pk_col_name = strdup(pk_col_name);
    assert(hash_table->pk_col_name != NULL);
    // add buckets as rows are inserted, there are 67 NULL buckets reserved

    // set hash table of this table
    new_tb->hash_table = hash_table;

    // prints success message
    fprintf(stdout, "table '%s' created successfuly with %d column(s).\n", new_tb_name, col_count);
    free(pk_col_name);
    pk_col_name = NULL;
}