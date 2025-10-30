/*
Date of creation : 16/10/2025
Description : Main entry point of the program
Group 2 ESGI 2A3
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "main.h"
#include "ui/parser.h"
#include "db/db.h"
#include "clean/clean.h"
#include "init/init.h"

void print_divider(){
    for(int i=0; i<20; i++) printf("-");
    printf("\n");
}

int main(int argc, char **argv){
    char import_export_line[5];
    char import_confirm;
    char export_confirm;
    char cmd_buffer[MAX_CMD_SIZE];
    char* cmd_input = NULL;

    Query* parser_output = NULL;
    Response* db_response = NULL;

    printf("Welcome to MiniDB !\n");
    printf("The Final Project developed in C by Minh Cat, Paco, Bamba. 2A3 ESGI 2025-2026.\n");
    printf("Please refer to README.md for all usage and all other information.\n");
    print_divider();

    // Prompt for file import
    printf("Do you want to import an existing database, do it now or never (y/n) : ");
    fgets(import_export_line, sizeof(import_export_line), stdin);
    import_confirm = import_export_line[0];
    
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
        if(parser_output->cmd_type == EXIT){ 
            free_current_cmd(&cmd_input, &parser_output);
            break;
        }

        // Check invalid syntax
        if(parser_output->cmd_type == INVALID && parser_output->syntax_message){
            printf("%s\n", parser_output->syntax_message);
            free_current_cmd(&cmd_input, &parser_output);
            continue;
        }


        // Execute command
        
        switch (parser_output->cmd_type)
        {
        case CREATE:
            // Call create() 
            db_response = create_table(parser_output);
            break;
        // case INSERT:
        //     // Call insert() of db

        //     //placeholder
        //     //no need to init response, it will be init in db functions
            
        //     printf("INSERT is called\n");
        //     break;
        // case SELECT:
        //     // Call select() of db

        //     //placeholder
        //     //no need to init response, it will be init in db functions
            
        //     printf("SELECT is called\n");
        //     break;
        // case DELETE:
        //     // Call delete() of db

        //     //placeholder
        //     //no need to init response, it will be init in db functions
            
        //     printf("DELETE is called\n");
        //     break;
        // case DROP:
        //     // Call drop() of db : 

        //     //placeholder
        //     //no need to init response, it will be init in db functions
            
        //     printf("DROP is called\n");
        //     break;
        default:
            printf("Command invalid, please check the syntax.\n");
            break;
        }

        // Check execution status
        if(db_response->status == FAILURE && db_response->message){
            printf("%s\n", db_response->message);
            free(db_response);
            free_current_cmd(&cmd_input, &parser_output);
            continue;
        } else if(db_response->status == SUCCESS && db_response->message){
            printf("Executed : %s\n", db_response->message);
        }
            

        // free before getting new command
        free(db_response);
        free_current_cmd(&cmd_input, &parser_output);
    }

    printf("Do you want to export the database, do it now or never (y/n) : ");
    fgets(import_export_line, sizeof(import_export_line), stdin);
    export_confirm = import_export_line[0];
    if(export_confirm == 'y' || export_confirm == 'Y'){
        // Call export func from file folder
    }else {
        print_divider();
        printf("Database exportation aborted.\n");
        print_divider();
    }

    
    // Call to functions in clean.c to free all db struct before exit

    printf("Goodbye !");
    exit(EXIT_SUCCESS);
}

