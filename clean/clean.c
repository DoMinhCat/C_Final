/*
Date of creation : 18/10/2025
Description : Free functions for all pointers to call before exit
Group 2 ESGI 2A3
*/

#include <stdlib.h>

#include "clean.h"


/*IMPORTANT : 
- Free all dynamic pointers in a struct before freeing the struct
*/ 

void free_query(Query** query){
    int i;
    if(!*query || !query) return;

    switch ((*query)->cmd_type)
    {
    case CREATE:
        //free col_list
        for(i=0; i<(*query)->params.create_params.col_count; i++){
            free((*query)->params.create_params.col_list[i]);
            (*query)->params.create_params.col_list[i] = NULL;
        }
        free((*query)->params.create_params.col_list);
        (*query)->params.create_params.col_list = NULL;

        //free type_list
        free((*query)->params.create_params.type_list);
        (*query)->params.create_params.type_list = NULL;

        (*query)->params.create_params.col_count = 0;
        break;
    case INSERT:
        //free col_list
        for(i=0; i<(*query)->params.insert_params.col_count; i++){
            free((*query)->params.insert_params.col_list[i]);
            (*query)->params.insert_params.col_list[i] = NULL;
        }
        free((*query)->params.insert_params.col_list);
        (*query)->params.insert_params.col_list = NULL;
        (*query)->params.insert_params.col_count = 0;

        //free data_list
        for(i=0; i<(*query)->params.insert_params.col_count; i++){
            free((*query)->params.insert_params.data_list[i]);
            (*query)->params.insert_params.data_list[i] = NULL;
        }
        free((*query)->params.insert_params.data_list);
        (*query)->params.insert_params.data_list = NULL;
        break;
    case DELETE:
        // no dynamic array for now
        break;
    case SELECT:
        /* code */
        break;
    default:
        // for case INVALID or EXIT, but initialised with NULL, so no need to free
        break;
    }

    free(*query);
    *query = NULL;
}

void free_current_cmd(char** cmd_string, Query** query){
    free(*cmd_string);
    *cmd_string = NULL;
    free_query(query);
}
