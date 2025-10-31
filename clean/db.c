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
    free(col->next_col);
    col->name = NULL;
    col->next_col = NULL;
}
    void free_row(Row* row, int col_count){
    int i;
    
    for(i=0; i<col_count; i++){
        free(row->data_field[i]);
        row->data_field[i] = NULL;
    }
    free(row->data_field);
    row->data_field = NULL;

    free(row->next_row);
    row->next_row = NULL;
}
void free_table(); //free col and row from last to first