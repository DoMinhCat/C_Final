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

int* get_fk_col_list_index(Query* query);
Col* get_last_col(Col* first_col);
Table* get_last_table(Table* first_table);

#endif