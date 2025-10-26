/*
Date of creation : 18/10/2025
Description : Free functions for all pointers to call before exit
Group 2 ESGI 2A3
*/

#include <stdlib.h>

#include "clean.h"


/*IMPORTANT : 
- Free all dynamic pointers in a struct before freeing the struct
*/ 

void free_query(Query** query){
    if(!query) return;
    free(query);
    query = NULL;
}

void free_current_cmd(char** cmd_string, Query** query){
    free(cmd_string);
    cmd_string = NULL;
    free(query);
    query = NULL;
}
