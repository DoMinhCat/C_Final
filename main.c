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
#include "hash/hash.h"
#include "file/file.h"

void print_divider(){
    for(int i=0; i<20; i++) printf("-");
    printf("\n");
}
void flush_extra(){
    int c = getchar();
    if (c != '\n' && c != EOF) {
        while ((c = getchar()) != '\n' && c != EOF);
    }
}

int main(int argc, char **argv){
    char import_export_choice;
    char cmd_buffer[MAX_CMD_SIZE];
    char* cmd_input = NULL;
    char confirm;
    char file_buffer[MAX_FILE_NAME];
    char* export_name = NULL;
    char c;

    Query* parser_output = NULL;

    printf("Welcome to MiniDB !\n");
    printf("The Final Project developed in C by Minh Cat, Paco, Bamba. 2A3 ESGI 2025-2026.\n");
    printf("Please refer to README.md for all usage and all other information.\n");
    print_divider();

    // Prompt for file import
    do{
        printf("Do you want to import an existing database, do it now or never (y/n) : ");
        scanf(" %c", &import_export_choice);
        flush_extra();
    } while (import_export_choice != 'y' && import_export_choice != 'Y' && import_export_choice != 'n' && import_export_choice != 'N');
    
    
    if(import_export_choice == 'y' || import_export_choice == 'Y'){
        print_divider();
        printf("Import confirmed\n");
        print_divider();
        
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
        if (cmd_input == NULL || strcmp("long", cmd_input) == 0) continue;

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
        case SHOW:
            show(parser_output);
            break;
        case DESCRIBE:
            describe_table(parser_output);
            break;
        case CREATE:
            create_table(parser_output);
            break;
        case INSERT:
            insert(parser_output);
            break;

        case SELECT:
            // Call select() of db
            
            printf("SELECT is called\n");
            break;
        case DELETE:
            // without WHERE clause
            if(!parser_output->params.delete_params.condition_column){
                printf("Confirm deletion of all rows from '%s' table, press 'y' to proceed (cancel on default): ", parser_output->params.delete_params.table_name);
                confirm = getchar();
                flush_extra();
                
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
            flush_extra();

            if(confirm == 'y'){
                drop_table(parser_output);
            }else printf("Execution of DROP statement aborted.\n");
            break;
        default:
            printf("Invalid command, please check the syntax.\n");
            break;
        }          

        // in both case success and failure, msg will be printed by db function

        // free before getting new command
        free_current_cmd(&cmd_input, &parser_output);
    }

    // Prompt for db export
    do
    {
        printf("Do you want to import export the current database, do it now or never (y/n) : ");
        scanf(" %c", &import_export_choice);
        flush_extra();
    } while (import_export_choice != 'y' && import_export_choice != 'Y' && import_export_choice != 'n' && import_export_choice != 'N');

    if(import_export_choice == 'y' || import_export_choice == 'Y'){
        print_divider();
        do{
            printf("Choose a name for your export file: ");
            export_name = read_file_name(file_buffer);
        } while(!export_name);
        
        printf("Exporting database...\n");
        // export_db(export_name, first_table);
        
        printf("Database exported successfully to '%s'.\n", export_name);
        free(export_name);
        export_name = NULL;
        print_divider();
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

