/*
Date of creation : 06/11/2025
Description : Code of WHERE clause for SELECT and DELETE
Group 2 ESGI 2A3
*/

#include "db.h"

// idea: return a list of pointers to rows that satisfy condition?
int where_int(Table* table, char* col_name, const int condition){
    Row* current_row = table->first_row;
    int col_index = get_col_index(table, col_name);
}

int where_str(Table* table, Col* col_name, const char* condition){
    Row* current_row = table->first_row;
    int col_index = get_col_index(table, col_name);
}

int where_double(Table* table, Col* col_name, const double condition){
    Row* current_row = table->first_row;
    int col_index = get_col_index(table, col_name);
}