/*
Date of creation : 17/10/2025
Description : All structures of database are declared here
Group 2 ESGI 2A3
*/

#ifndef DB_H
#define DB_H
#include <stdio.h>
#include <stdbool.h>

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

// Database management functions
Table* create_table(const char* table_name, char** col_names, ColType* col_types, int col_count);
bool drop_table(const char* table_name);
Table* get_table(const char* table_name);
Table* get_first_table(void);
void set_first_table(Table* table);

// Data manipulation functions
bool delete_rows(Table* table, const char* column_name, const char* value);
bool insert_row(Table* table, char** values, int value_count);
Row* select_rows(Table* table, const char* column_name, const char* value);

#endif