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
    table->first_row = NULL;
    table->first_col = NULL;
    table->next_table = NULL;

    return table;
}

Col* init_col(){
    Col* col = NULL;

    assert((col = (Col*)malloc(sizeof(Col))) != NULL);
    col->name = NULL;
    col->constraint = NONE;
    col->type = INT;
    col->next_col = NULL;

    return col;
}
