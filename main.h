/*
Date of creation : 17/10/2025
Description : Universal structures
Group 2 ESGI 2A3
*/

#ifndef MAIN_H
#define MAIN_H
#define TABLE_NAME_MAX 50

typedef enum{
    CREATE,
    INSERT,
    DELETE,
    SELECT,
    DROP,
    EXIT
} CommandType;

// For Create table function
typedef struct{
    char table_name[TABLE_NAME_MAX];
    char **col_list; // list of column names passed in the query
    CommandType *type_list;  // list of types corresponding to column
    int col_count;          // number of columns
    // TODO : what about foreign key and primary key
} CreateParams;

// For Insert function
typedef struct{
    char table_name[TABLE_NAME_MAX];
    char **col_list; // list of column names passed in the query
    void **data_list; // list of input for each column
    
    // TODO: define other necessary params
} SelectParams;

typedef struct {
    char table_name[TABLE_NAME_MAX];
    char condition_column[TABLE_NAME_MAX];
    char condition_value[TABLE_NAME_MAX];
} DeleteParams;

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
        SelectParams select_params;
        // Other structs of other types of commands
        DeleteParams delete_params;
        DropParams drop_params;
    } params;

    // set this if there is a syntax error that the parser detected
    char syntax_message[100];
} Query;








void print_divider();

#endif
