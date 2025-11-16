/*
Date of creation : 16/11/2025
Description : Code for database export to file 
Group 2 ESGI 2A3
*/

#include <stdio.h>
#include <stdbool.h>

#include "../db/db.h"
#include "../hash/hash.h"
#include "../global_var.h"

/*
NOTE: export structure for table to fwrite in order:
    # metadata
    Table name
    Num of Cols
    Num of Rows
    Num of Hash Tables

    Next_id of table (int)
    Col one by one:
        name, type, constraint
        strlen of refer_table, refer_table (str)   IMPORTANT: for all strings read later need to alloc strlen + 1 and set \0 at the end
        strlen of refer_col, refer_col (str)

    Row one by one:
        int_count
        double_count
        str_count
        int items
        double items
        str items

    Hash table one by one:
        strlen of col_name, col_name

        loop 67 times for each bucket:
            Loop and write num of nodes that handle collision
            for prev_row and row, write the row position (int), not the pointer
            strlen of original_value and original_value (str)

*/ 

void export_col(FILE* output_file, Col* col){
    // TODO
}

void export_row(FILE* output_file, Row* row){
    // TODO
}

void export_hash_node(FILE* output_file, Node* hash_node){
    // TODO
}

void export_hash_table(FILE* output_file, HashTable* ht_to_export){
    // TODO
}

void export_table(FILE* output_file, Table* table){
    // TODO
}

void export_db(char* output_file_name, Table* first_table){
    // export the whole database to a binary file

    FILE* output_file = NULL;
    Table* current_table = first_table;
    char* extension = ".bin";
    int i;

    //add the .bin extension
    output_file_name = realloc(output_file_name, strlen(output_file_name) + strlen(extension) + 1);
    assert(output_file_name!=NULL);
    strcat(output_file_name, extension);

    // open file to write
    output_file = fopen(output_file_name, "wb");
    if(!output_file){
        fprintf(stderr, "Export error: failed to create '%s.bin'.\n", output_file_name);
        return;
    }
    
    // write table one by one
    for(i=0; i<table_count; i++){
        export_table(output_file, current_table);
        current_table = current_table->next_table;
    }
}