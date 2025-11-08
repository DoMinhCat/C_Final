/*
Date of creation : 03/11/2025
Description : Code for drop table operation
Group 2 ESGI 2A3
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "db.h"
#include "helper_db.h"
#include "../main.h"
#include "../ui/parser.h"
#include "../init/init.h"
#include "../global_var.h"
#include "../clean/clean.h"

void drop_table(Query* query) {
    Table* current_table;
    Table* prev_table = NULL;
    Col* current_col = NULL;
    Col* next_col = NULL;
    Table* tb_to_del = NULL;
    char* table_name = NULL;

    int i;
    int table_count = query->params.drop_params.table_count;
    
    // Cat's note : removed check for non existing table, this is already checked in parser

    // Loop through each table provided in query, check and free one by one 
    for(i=0; i<table_count; i++){
        table_name = query->params.drop_params.table_list[i];
    
        // check existence
        if(!table_exists(table_name)) return;

        // Check if any other table has a foreign key col references to current table
        current_table = first_table;
        while(current_table != NULL) {
            // no need to check itself 
            if(strcmp(current_table->name, table_name) != 0){
                current_col = current_table->first_col;
                while(current_col != NULL) {
                    // return error if col refer to table to delete
                    if(current_col->constraint == FK && strcmp(current_col->refer_table, table_name) == 0) {
                        fprintf(stderr, "Execution error: '%s' 'is referenced by column '%s' of table '%s.\n", table_name, current_col->name, current_table->name);
                        return;
                    }
                    current_col = current_col->next_col;
                }
            } 
            current_table = current_table->next_table;
        }

        // done checking, now drop table
        current_table = first_table;
        prev_table = NULL;
        while(current_table != NULL) {
            //set prev_table
            if(current_table == first_table) prev_table = NULL; 

            if(strcmp(current_table->name, table_name) == 0) {
                // table found
                tb_to_del = current_table; 

                // tb to del is the first table
                if(tb_to_del == first_table) first_table = tb_to_del->next_table;
                //if tb to del is mid/last
                else prev_table->next_table = tb_to_del->next_table;
                
                // advance to next cuz we will lose pointer to this table to be deleted
                current_table = current_table->next_table;

                // drop table
                free_table(tb_to_del);
                tb_to_del = NULL;
                break;
            } 
            //if current_table is not to be deleted, advance pointers normally
            prev_table = current_table;
            current_table = current_table->next_table;
        }
    }

    // print result
    fprintf(stdout, "Executed: %s ", table_count > 1 ? "tables" : "table");
    for (i = 0; i < table_count; i++) {
        fprintf(stdout, "'%s'%s", query->params.drop_params.table_list[i], (i < table_count - 1) ? ", " : ""); // add other table names
    }
    fprintf(stdout, " dropped successfully.\n");

    return;
}