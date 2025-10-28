/*
Date of creation : 28/10/2025
Description : Clean initialisation of Table, Col, Row structure
Group 2 ESGI 2A3
*/

#include <stdlib.h>
#include <assert.h>

#include "init.h"

// safely initialise Query struct
Table* init_table(){
    Table* table = NULL;

    assert((table = (Table*)malloc(sizeof(Table))) != NULL);
    (*table)->name = NULL;
    (*table)->first_row = NULL;
    (*table)->first_col = NULL;
    (*table)->next_table = NULL;

    return table;
}
