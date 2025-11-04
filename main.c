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
#include "global_var.h"

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
    char confirm;

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
        // if nothing or command too long (read_cmd returns NULL)
        if (cmd_input == NULL){
            continue;
        } else if(strcmp("long", cmd_input) == 0){
            printf("\n");
            continue;
        }

        // Call parser from ui folder analyze command
        parser_output = parse_cmd(cmd_input);

        // Check exit/quit
        if(parser_output->cmd_type == EXIT){ 
            free_current_cmd(&cmd_input, &parser_output);
            break;
        }

        // Check invalid syntax
        if(parser_output->cmd_type == INVALID){
            free_current_cmd(&cmd_input, &parser_output);
            continue;
        }

        // Execute commands
        switch (parser_output->cmd_type){
        case CREATE:
            // Call create() 
            //db_response = create_table(parser_output);

            printf("CREATE is called\n");
            break;

        case INSERT:
            // Call insert() of db

            //placeholder
            //no need to init response, it will be init in db functions
            
            printf("INSERT is called\n");
            break;

        case SELECT:
            // Call select() of db

            //placeholder
            //no need to init response, it will be init in db functions
            
            printf("SELECT is called\n");
            break;
        case DELETE:
            // without WHERE clause
            if(!parser_output->params.delete_params.condition_column){
                printf("Confirm deletion of all rows from '%s' table, press 'y' to proceed (cancel on default): ", parser_output->params.delete_params.table_name);
                confirm = getchar();
                // flush extra chars
                while ((getchar()) != '\n' && getchar() != EOF);
                if(confirm == 'y'){
                    // call delete
                    printf("DELETE is called\n");
                }else printf("Execution of DELETE statement aborted.\n");
            }
            // execute normally if there is WHERE
            else{
                // call delete
                printf("DELETE is called\n");
            }
            break;

        case DROP:
        // ask for confirmation
            printf("Confirm deletion of %d %s, press 'y' to proceed (cancel on default): ", parser_output->params.drop_params.table_count, parser_output->params.drop_params.table_count>1?"tables":"table");
            confirm = getchar();
            // flush extra chars
            while ((getchar()) != '\n' && getchar() != EOF);

            if(confirm == 'y'){
                // call to drop
                printf("DROP is called\n");
            }else printf("Execution of DROP statement aborted.\n");
            break;
        default:
            printf("Invalid command, please check the syntax.\n");
            break;
        }

        // Check execution status

        /*
        if(db_response->status == FAILURE && db_response->message){
            printf("%s\n", db_response->message);
            free(db_response);
            free_current_cmd(&cmd_input, &parser_output);
            continue;
        } else if(db_response->status == SUCCESS && db_response->message){
            printf("Executed : %s\n", db_response->message);
        }
            */
            

        // free before getting new command

        if(db_response) free(db_response); // in case no confirm for DROP/DELETE
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
    free_db(first_table);

    printf("Goodbye !");
    exit(EXIT_SUCCESS);
}

