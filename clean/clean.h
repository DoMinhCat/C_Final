/*
Date of creation : 26/10/2025
Description : Prototypes for clean.c
Group 2 ESGI 2A3
*/

#ifndef CLEAN_H
#define CLEAN_H

typedef struct Response Response;  
typedef struct Query Query;  

void free_current_cmd(char** cmd_string, Query** query);
void free_query(Query** query);

#endif