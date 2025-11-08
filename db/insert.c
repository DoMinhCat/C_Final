/*
Date of creation : 08/11/2025
Description : Code of INSERT statement
Group 2 ESGI 2A3
*/

#include <stdlib.h>
#include <stdio.h>

#include "db.h"
#include "helper_db.h"
#include "../ui/parser.h"

void insert(Query* query){
    char** col_list = query->params.insert_params.col_list;
    char* col_name = NULL;
    int col_count = query->params.insert_params.col_count;
    void** data_list = query->params.insert_params.data_list;
    char* tb_name = query->params.insert_params.table_name;
    Table* table = get_table_by_name(tb_name);
    ColType* type_list = NULL; //(ColType*)malloc(sizeof(ColType) * col_count); // TODO: free at early return
    ColType type;
    Col* current_col = table->first_col;
    bool col_exist;

    int i;

    // check table exist
    if(!table_exists(tb_name)) return;

    // checks for col_list
    for(i=0; i<col_count; i++){
        col_name = col_list[i];
        // Do all checks in 1 loop instead of calling helper to avoid additional loop. Loop through cols of table
        for(i=0; i<table->col_count; i++) {
            col_exist = false;

            // check existence of col
            if (strcmp(current_col->name, col_list[i]) == 0) {
                col_exist = true;
                // TODO: found col, check type same with data type
                // if err: return, if ok: put type in type list for later insert
                type = current_col->type;

                // TODO: check PK case, can't insert value already existing, should create a helper for future UNIQUE constraint
                // hash 
            }

            current_col = current_col->next_col;
        }

        if(!col_exist){
            fprintf(stderr, "Column '%s' not found.\n", col_name);
            return;
        }
    



    }

    // start inserting

    //TODO : set id, check id, auto increment (see ideas in README)
    // TODO : hash id then add to hash table of this table
    // TODO : cast data fields to its correct type and add to row


}