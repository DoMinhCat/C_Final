/*
Date of creation : 29/10/2025
Description : Helper functions protypes are declared here
Group 2 ESGI 2A3
*/

#ifndef HELPER_DB_H
#define HELPER_DB_H

typedef struct Col Col;
typedef struct Table Table;
typedef struct Query Query;
typedef struct Row Row;

int* get_fk_col_list_index(Query* query);
Col* get_last_col(Col* first_col);
Table* get_last_table(Table* first_table);

Table* get_table_by_name(const char* table_name);
Col* get_col_by_name(Table* table, const char* col_name);

// New functions for select
Table* find_table(const char* table_name);
Col* find_col(Table* table, const char* col_name);
int get_col_list_index(Table* table, Row* row, const char* col_name, ColType* col_type);
void* get_col_value(Table* table, Row* row, const char* col_name, ColType* col_type);
void format_value(ColType type, void* value);
#endif