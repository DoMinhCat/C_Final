/*
Date of creation : 16/11/2025
Description : Code for database export to file 
Group 2 ESGI 2A3
*/

#include <stdio.h>
#include <stdbool.h>

#include "../db/db.h"
#include "../hash/hash.h"

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

void export_table(FILE* output_file, Col* col){
    
}

void export_table(FILE* output_file, Row* row){
    
}

void export_hash_node(FILE* output_file, Node* hash_node){

}

void export_hash_table(FILE* output_file, HashTable* ht_to_export){

}

void export_table(FILE* output_file, Table* table){
    
}

bool export_db(char* output_file_name, Table* first_table){
    FILE* output_file = NULL;


}