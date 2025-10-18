/*
Date of creation : 16/10/2025
Description : Main entry point of the program
Group 2 ESGI 2A3
*/

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){
    char import_confirm;


    printf("Welcome to MiniDB !\n");
    printf("The Final Project developed in C by Minh Cat, Paco, Bamba. 2A3 ESGI 2025-2026.\n");
    printf("Please refer to README.md for all usage and all other information.\n");

    // Prompt for file import
    printf("Do you want to import an existing database, do it now or never (y/n) : ");
    scanf("%1c", &import_confirm);
    if(import_confirm == 'y'){
        // Call import function from file folder
    }

    // Infinite loop to get user's command
    while(1){
        
        printf(">>> ");
        // Call functions from ui folder to read and analyze command

    }









    // Call to functions in clean.c to free all before exit
    exit(EXIT_SUCCESS);
}

