/*
Date of creation : 28/10/2025
Description : Clean initialisation of Table, Col, Row structure
Group 2 ESGI 2A3
*/

#include <stdlib.h>
#include <assert.h>

#include "init.h"
#include "../db/db.h"

// safely initialise Table struct
Table* init_table(){
    Table* table = NULL;

    assert((table = (Table*)malloc(sizeof(Table))) != NULL);
    table->name = NULL;
    table->first_row = init_row();
    table->first_col = init_col();
    table->next_table = NULL;
    table->col_count = 0;
    table->first_hash_table = NULL;
    table->next_id = 1;

    return table;
}

Col* init_col(){
    Col* col = NULL;
    assert((col = (Col*)malloc(sizeof(Col))) != NULL);

    col->name = NULL;
    col->refer_col = NULL;
    col->refer_table = NULL;
    col->constraint = NONE;
    col->type = INT;
    col->next_col = NULL;

    return col;
}

Row* init_row(){
    Row* row = NULL;
    assert((row = (Row*)malloc(sizeof(Row))) != NULL);

    row->int_list = NULL;
    row->str_list = NULL;
    row->double_list = NULL;

    row->next_row = NULL;    

    return row;
}