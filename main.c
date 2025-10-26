/*
Date of creation : 16/10/2025
Description : Main entry point of the program
Group 2 ESGI 2A3
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "ui/parser.h"
#include "main.h"
// Include all necessary .h files

void print_divider(){
    for(int i=0; i<20; i++) printf("-");
    printf("\n");
}

int main(int argc, char **argv){
    char import_confirm;
    char export_confirm;
    char cmd_buffer[MAX_CMD_SIZE];
    char* cmd_input;
    Query* parser_output;

    printf("Welcome to MiniDB !\n");
    printf("The Final Project developed in C by Minh Cat, Paco, Bamba. 2A3 ESGI 2025-2026.\n");
    printf("Please refer to README.md for all usage and all other information.\n");
    print_divider();

    // Prompt for file import
    printf("Do you want to import an existing database, do it now or never (y/n) : ");
    scanf(" %c", &import_confirm);
    
    if(import_confirm == 'y' || import_confirm == 'Y'){
        // Call import function from file folder
    }else {
        print_divider();
        printf("Database importation aborted.\n");
        print_divider();
    }

    // Infinite loop to get user's command
    while(1){
        
        // put the command in cmd_input
        printf(">>> ");
        cmd_input = read_cmd(cmd_buffer);

        // Call parser from ui folder analyze command
        parser_output = parse_cmd(cmd_input);

        // Check exit/quit
        if(parser_output->cmd_type == EXIT) break;

        // Check invalid syntax
        if(parser_output->cmd_type == INVALID && parser_output->syntax_message){
            printf("%s", parser_output->syntax_message);
            free(cmd_input);
            cmd_input = NULL;
            free(parser_output);
            parser_output = NULL;
            continue;
        }

        // free after each command
        free(cmd_input);
        cmd_input = NULL;
    }

    printf("Do you want to export the database, do it now or never (y/n) : ");
    scanf(" %c", &export_confirm);
    if(export_confirm == 'y' || export_confirm == 'Y'){
        // Call export func from file folder
    }else {
        print_divider();
        printf("Database exportation aborted.\n");
        print_divider();
    }

    
    // Call to functions in clean.c to free all before exit

    printf("Goodbye !");
    exit(EXIT_SUCCESS);
}

