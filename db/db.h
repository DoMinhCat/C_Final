/*
Date of creation : 17/10/2025
Description : All structures of database and db prototypes are declared here
Group 2 ESGI 2A3
*/

#ifndef DB_H
#define DB_H
#define MAX_TABLE_COUNT 50

#include <stdbool.h>

#include "../main.h"

// struct
typedef struct Row{
    void **data_field;
    struct Row *next_row;
} Row;

typedef struct Col{
    char* name;
    ColType type;
    ColConstraintType constraint;

    struct Col *next_col;
} Col;

typedef struct Table{
    char *name;
    Row *first_row;
    Col *first_col;

    int col_count; // to free row and col

    struct Table *next_table;
} Table;

// prototypes
typedef struct Response Response;  
typedef struct Query Query;  

Response* create_table(Query* query);
#endif