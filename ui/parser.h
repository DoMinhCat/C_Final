
/*
Date of creation : 26/10/2025
Description : Declaration of prototypes for parser.c, Query and Response struct
Group 2 ESGI 2A3
*/

#ifndef PARSER_H
#define PARSER_H

#define MAX_CMD_SIZE 1024
#define MAX_TOKEN_SIZE 257
#define TABLE_NAME_MAX 101

#include "../main.h"
typedef enum CommandType{
    CREATE,
    INSERT,
    DELETE,
    SELECT,
    DROP,
    EXIT,
    INVALID
} CommandType;

// For Create table function
typedef struct{
    char* table_name;

    char **col_list; // list of column names passed in the query
    int col_count;          // number of columns

    ColType *type_list;  // list of types corresponding order of col_list
    ColConstraintType *constraint_list; // list of constraint corresponding to order of col_list

    char** table_refer_list; //list of tables refered to by fk cols, doesnt follow order of col_list
    char** col_refer_list; // list of cols of refered tables refered to by fk cols, doesnt follow order of col_list
    int fk_count; // number of fk to free 2 lists above, done in clean.c
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
    char condition_column[TABLE_NAME_MAX]; // need to check input length
    char condition_value[MAX_TOKEN_SIZE]; // need to check input length
} DeleteParams;

typedef struct {
    char table_name[TABLE_NAME_MAX];
    char table_join_name[TABLE_NAME_MAX];

    char** col_list;
    int col_count; //to free col_list
    char first_col_on[TABLE_NAME_MAX];
    char second_col_on[TABLE_NAME_MAX];
    char condition_col[TABLE_NAME_MAX]; // need to check input length
    char condition_val[MAX_TOKEN_SIZE]; // need to check input length
} SelectParams;

typedef struct {
    char** table_list;
    int table_count; // to free table_list
} DropParams;

typedef enum{
    SUCCESS,
    FAILURE
} ResponseStatus;

// DB functions return this struct for error/success in db operations
typedef struct Response{
    ResponseStatus status;
    char message[100];
} Response;

typedef struct Query
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


char* read_cmd(char* cmd_buffer);

void parse_delete(Query** query);
void parse_drop(Query** query);
void parse_select(Query** query);
void parse_insert(Query** query);
void parse_create(Query** query);
Query* parse_cmd(char* cmd);

#endif


