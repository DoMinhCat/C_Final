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

typedef enum{
    INT,
    DOUBLE,
    STRING    
} ColType;

typedef struct Col{
    char name[30];
    ColType type;
    bool is_fk;
    bool is_pk;

    struct Col *next_col;
} Col;

typedef struct Table{
    char *name;
    Row *first_row;
    Col *first_col;

    struct Table *next_table;
} Table;



// defined in main.c
extern Table* first_table;
extern Row* first_row;
extern Col* first_col;


// prototypes
Response* create_table(Query query);
#endif