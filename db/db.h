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

typedef struct HashTable HashTable;

// struct
typedef struct Row{
    void **data_field; // double pointer in case data is string 
    struct Row *next_row;
} Row;

typedef struct Col{
    char* name;
    ColType type;
    ColConstraintType constraint;

    char* refer_table; // table that fk col references
    char* refer_col; // col of table that fk col references

    struct Col *next_col;
} Col;

typedef struct Table{
    char *name;
    Row *first_row;
    Col *first_col; 
    HashTable* hash_table; // pointer to the hash table of this table

    int col_count; // to free row and col

    struct Table *next_table;
} Table;

typedef struct Query Query;  

// prototypes
void create_table(Query* query);
void drop_table(Query* query);
void select_data(Query* query);

void describe_table(Query* query);
void show(Query* query);

#endif