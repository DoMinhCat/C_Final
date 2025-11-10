/*
Date of creation : 17/10/2025
Description : All structures of database and db prototypes are declared here
Group 2 ESGI 2A3
*/

#ifndef DB_H
#define DB_H
#define MAX_TABLE_COUNT 100
#define MAX_COL_COUNT 50
#define WARNING_ROW_COUNT 10000
#define MAX_STR_LEN 256

#include <stdbool.h>

#include "../main.h"

typedef struct HashTable HashTable;

// struct
typedef struct Row{
    int** int_list; // unset int will be NULL, int_list[int_count][1]
    char** str_list;
    double** double_list; // unset double will be NULL, double_list[double_count][1]

    int int_count;
    int str_count;
    int double_count;

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
    HashTable* first_hash_table; // linked list of hash tables of this table
    int next_id;

    int col_count; // to free row and col

    struct Table *next_table;
} Table;

typedef struct Query Query;  

// prototypes
void create_table(Query* query);
void drop_table(Query* query);
void insert(Query* query);

void describe_table(Query* query);
void show(Query* query);
#endif