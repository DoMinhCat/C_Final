/*
Date of creation : 18/10/2025
Description : Free functions for all pointers to call before exit
Group 2 ESGI 2A3
*/

#include <stdlib.h>

#include "clean.h"

void free_current_cmd(char** cmd_string, Query** query){
    free(cmd_string);
    cmd_string = NULL;
    free(query);
    query = NULL;
}
