/*
Date of creation : 2/11/2025
Description : Declaration of prototypes for helper functions of parser
Group 2 ESGI 2A3
*/

#ifndef HELPER_UI_H
#define HELPER_UI_H

#include <stdbool.h>

typedef struct Query Query; 
typedef enum CommandType CommandType; 

bool contains_visible_char(char* str_to_check);
void check_end_of_cmd(char* last_token, Query** query, char* current_stmt);
bool contain_key_word(char* token, char* keyword, Query** query, char* current_stmt);
#endif