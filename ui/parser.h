
/*
Date of creation : 26/10/2025
Description : Declaration of prototypes for parser.c
Group 2 ESGI 2A3
*/

#include "../main.h"
#ifndef PARSER_H
#define PARSER_H

#define MAX_CMD_SIZE 1024
#define MAX_TOKEN_SIZE 256

Query* parse_command(char* cmd);
char* read_cmd(char* cmd_buffer);
#endif