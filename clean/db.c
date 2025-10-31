/*
Date of creation : 31/10/2025
Description : Free functions for Row, Col and Table pointers to call before exit
Group 2 ESGI 2A3
*/

#include <stdlib.h>

#include "clean.h"
#include "../db/db.h"

void free_col(Col* col){
    free(col->name);
    col->name = NULL;
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
void free_table(Table* table){
    // this is to clean only 1 table, for all table need to free from last table to first
    int i;
    Col* current_col = table->first_col;
    Col* tmp_col; // temporary to free current col
    Row* current_row = table->first_row;
    Row* tmp_row;

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

    free(table);
    table = NULL;
} 

// WARNING : must not free pointer to the next one