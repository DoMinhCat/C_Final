/*
Date of creation : 16/10/2025
Description : Definition of functions parsing user's commands
Group 2 ESGI 2A3
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "parser.h"
#include "../init/init.h"

char* read_cmd(char* cmd_buffer){ 
    int c;

    if(fgets(cmd_buffer, MAX_CMD_SIZE, stdin) == NULL || strcmp(cmd_buffer, "\n") ==0) return NULL;

    // check if cmd is too long
    if(strlen(cmd_buffer) > 0 && cmd_buffer[strlen(cmd_buffer)-1] != '\n'){
        while ((c = getchar()) != '\n' && c != EOF); // flush the rest of cmd in stdin if cmd too long
        printf("Command allows %d characters at max.", MAX_CMD_SIZE-1);
        return "long";
    } 

    // remove trailing newline
    cmd_buffer[strcspn(cmd_buffer, "\n")] = '\0';

    char* cmd_string = malloc(strlen(cmd_buffer)+1);
    assert(cmd_string != NULL);
    strcpy(cmd_string, cmd_buffer);
    return cmd_string;
}

Query* parse_cmd(char* cmd) {
    Query* query = init_query();

    char* token;

    token = strtok(cmd, " \t");

    // no need to handle null cmd, already handled in main.c
    if(strcasecmp(token, "DELETE") == 0) parse_delete(&query);
    else if(strcasecmp(token, "DROP") == 0) parse_drop(&query);
    else if(strcasecmp(token, "CREATE") == 0) parse_create(&query);
    else if(strcasecmp(token, "SELECT") == 0) parse_select(&query);
    else if(strcasecmp(token, "INSERT") == 0) parse_insert(&query);
    else if(strcasecmp(token, "SHOW") == 0) parse_show(&query);



    // exit/quit case 
    else if(strcasecmp(token, "EXIT") == 0 || strcasecmp(token, "QUIT") == 0) query->cmd_type = EXIT;
    
    // first word is not one of the accepted command (create, select, insert,...)
    else {
        query->cmd_type = INVALID;
        fprintf(stderr, "Syntax error: command '%s' not found, please check the syntax.\n", token);
    }

    return query;
}

