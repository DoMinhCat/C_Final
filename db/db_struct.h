/*
Date of creation : 17/10/2025
Description : All structures of database are declared here
Group 2 ESGI 2A3
*/

#ifndef DB_STRUCT_H
#define DB_STRUCT_H
#include <stdio.h>

typedef struct Row{
    void **data_field;
    struct Row *next_row;
} Row;

typedef enum{
    INT,
    DOUBLE,
    STRING    
} ColType;

typedef struct Col{
    char name[30];
    ColType type;

    struct Col *next_col;
} Col;

typedef struct Table{
    char name[30];
    Row *first_row;
    Col *first_col;

    struct Table *next_table;
} Table;

#endif