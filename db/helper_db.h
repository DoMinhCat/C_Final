/*
Date of creation : 29/10/2025
Description : Helper functions protypes are declared here
Group 2 ESGI 2A3
*/
#include <stdbool.h>

#include "../main.h"

#ifndef HELPER_DB_H
#define HELPER_DB_H

typedef struct Col Col; 
typedef struct Row Row; 
typedef struct Table Table; 
typedef struct Query Query; 
typedef struct HashTable HashTable; 

// get pointer functions
int* get_fk_col_list_index(Query* query);
// Col* get_last_col(Col* first_col);
Row* get_last_row(Row* first_row);
Table* get_last_table(Table* first_table);
// HashTable* get_last_hash_table(HashTable* first_ht);
int get_data_list_index(Table* table, char* col_name);
Table* get_table_by_name(char* table_name);
Col* get_col_by_name(Table* table, char* col_name);
HashTable* get_ht_by_col_name(HashTable* first_ht, char* col_name);
int compare_double(double val1, double val2); 

// check functions
bool table_exists(char* table_name);
bool col_exists(Table* table, char* col_name);
bool refer_val_exists(char* str_to_check, int val_to_check, char* ref_table_name, char* ref_col_name);
bool pk_value_is_unique(char* str_to_check, int val_to_check, HashTable* hash_tab, char* constraint);

char* int_to_str(int val);

#endif