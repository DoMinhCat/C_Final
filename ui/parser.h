
/*
Date of creation : 26/10/2025
Description : Declaration of prototypes for parser.c
Group 2 ESGI 2A3
*/

#include "../main.h"
#include "../main.h"
#include "../db/db.h"

#ifndef PARSER_H
#define PARSER_H

#define MAX_CMD_SIZE 1024
#define MAX_TOKEN_SIZE 256

Query* init_query();

Query* parse_cmd(char* cmd);
char* read_cmd(char* cmd_buffer);
#endif