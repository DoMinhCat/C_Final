/*
Date of creation : 26/10/2025
Description : Prototypes for clean.c
Group 2 ESGI 2A3
*/

#include "../db/db.h"
#include "../main.h"
#include "../hash/hash.h"

void free_current_cmd(char** cmd_string, Query** query);
void free_query(Query** query);