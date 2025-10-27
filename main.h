/*
Date of creation : 17/10/2025
Description : Universal structures
Group 2 ESGI 2A3
*/

#ifndef MAIN_H
#define MAIN_H
#define TABLE_NAME_MAX 101
#define MAX_TOKEN_SIZE 257

#include "db/db.h"

typedef enum{
    CREATE,
    INSERT,
    DELETE,
    SELECT,
    DROP,
    EXIT,
    INVALID
} CommandType;

typedef enum{
    NONE,
    FK,
    PK
} ColConstraintType;

// For Create table function
typedef struct{
    char table_name[TABLE_NAME_MAX];

    char **col_list; // list of column names passed in the query
    int col_count;          // number of columns

    ColType *type_list;  // list of types corresponding to column
    ColConstraintType *constraint_list; // list of constraint corresponding to order of col_list
} CreateParams;

// For Insert function
typedef struct{
    char table_name[TABLE_NAME_MAX];

    char **col_list; // list of column names passed in the query
    int col_count;          // number of columns for free operation

    void **data_list; // list of input for each column
    
    // TODO: define other necessary params
} InsertParams;

typedef struct {
    char table_name[TABLE_NAME_MAX];
    char condition_column[TABLE_NAME_MAX];
    char condition_value[MAX_TOKEN_SIZE];
} DeleteParams;

typedef struct {
    char table_name[TABLE_NAME_MAX];
    
    // add more later
} SelectParams;

typedef struct {
    char table_name[TABLE_NAME_MAX];
} DropParams;

typedef enum{
    SUCCESS,
    FAILURE
} ResponseStatus;

// DB functions return this struct for error/success in db operations
typedef struct{
    ResponseStatus status;
    char message[100];
} Response;

typedef struct 
{
    CommandType cmd_type; 

    // take one of these params based on cmd_type
    union{
        CreateParams create_params;
        InsertParams insert_params;
        // Other structs of other types of commands
        DeleteParams delete_params;
        DropParams drop_params;
        SelectParams select_params;
    } params;

    // set this if there is a syntax error that the parser detected
    char syntax_message[100];
} Query;




void print_divider();

#endif
