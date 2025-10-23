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
    CommandType *type_list  // list of types corresponding to column

    // TODO : what about foreign key and primary key
} CreateParams;

// For Insert function
typedef struct{
    char table_name[TABLE_NAME_MAX];
    char **col_list; // list of column names passed in the query
    void **data_list; // list of input for each column
    
    // TODO: define other necessary params
} SelectParams;

typedef struct 
{
    CommandType cmd_type; 
    union{
        CreateParams create_params;
        SelectParams select_params;
        // Other structs of other types of commands

    } params;

    // Error message to set if there is syntax error or parameter excede max num of chars allowed
    char err_msg[100];
} Query;

#endif
