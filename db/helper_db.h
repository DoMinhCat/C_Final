/*
Date of creation : 29/10/2025
Description : Helper functions protypes are declared here
Group 2 ESGI 2A3
*/
#include <stdbool.h>

#ifndef HELPER_DB_H
#define HELPER_DB_H

typedef struct Col Col; 
typedef struct Table Table; 
typedef struct Query Query; 

int* get_fk_col_list_index(Query* query);
Col* get_last_col(Col* first_col);
Table* get_last_table(Table* first_table);
int get_data_list_index(Table* table, char* col_name);

bool table_exists(char* table_name);
bool col_exists(Table* table, char* col_name);
bool is_unique_str(Table* table, char* col_name, char* value_to_check);
bool is_unique_double(Table* table, char* col_name, double value_to_check);
bool is_unique_int(Table* table, char* col_name, int value_to_check);

Table* get_table_by_name(const char* table_name);
Col* get_col_by_name(Table* table, const char* col_name);

#endif