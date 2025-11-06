/*
Date of creation : 31/10/2025
Description : Free functions for Row, Col and Table pointers to call before exit
Group 2 ESGI 2A3
*/

#include <stdlib.h>

#include "clean.h"
#include "../db/db.h"
#include "../hash/hash.h"

void free_col(Col* col){
    free(col->name);
    col->name = NULL;
    
    free(col->refer_table);
    col->refer_table = NULL;

    free(col->refer_col);
    col->refer_col = NULL;
    
    free(col);
    col = NULL;
}

void free_row(Row* row, int col_count){
    int i;
    
    for(i=0; i<col_count; i++){
        free(row->data_field[i]);
        row->data_field[i] = NULL;
    }
    free(row->data_field);
    row->data_field = NULL;

    free(row);
    row = NULL;
}

void free_node(Node* node, int col_count){
    free(node);
    node = NULL;
    // dont free row that node points to, row will be free when freeing table else we free row 2 times
}

void free_hash_table(HashTable* hash_table, int col_count){
    int i;
    Node* current_node = NULL;
    Node* tmp_node = NULL;

    free(hash_table->pk_col_name);
    hash_table->pk_col_name = NULL;

    // free Node linked list of each bucket
    for(i=0; i<HASH_TABLE_SIZE; i++){
        current_node = hash_table->bucket[i];
        while(current_node != NULL){
            tmp_node = current_node;
            // save pointer to next col, then free current pointer
            current_node =current_node->next_node;
            
            free_node(tmp_node, col_count);   
        }
        hash_table->bucket[i] = NULL;
    }

    free(hash_table);
    hash_table = NULL;
}

void free_table(Table* table){
    // this is to clean only 1 table
    int i;
    Col* current_col = table->first_col;
    Col* tmp_col; // temporary to free current col
    Row* current_row = table->first_row;
    Row* tmp_row;
    HashTable* hash_table = NULL;
    

    free(table->name);
    table->name = NULL;

    //free col 
    while(current_col != NULL){
        tmp_col = current_col;
        // save pointer to next col, then free current pointer
        current_col = current_col->next_col;
        
        free_col(tmp_col);   
    }

    //free row 
    while(current_row != NULL){
        tmp_row = current_row;
        // save pointer to next row, then free current pointer
        current_row = current_row->next_row;
        
        free_row(tmp_row, table->col_count);   
    }

    // free the associated hash table
    free_hash_table(table->hash_table, table->col_count);
    hash_table = NULL;

    free(table);
    table = NULL;
} 

void free_db(Table* first_table){
    // this func free all tables
    Table* current_table = first_table;
    Table* tmp_table = NULL;

    while(current_table!=NULL){
        tmp_table = current_table;
        current_table = current_table->next_table;

        free_table(tmp_table);
        tmp_table = NULL;
    }
}

// WARNING : must not free pointer to the next one