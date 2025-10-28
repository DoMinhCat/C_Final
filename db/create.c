/*
Date of creation : 17/10/2025
Description : Code for create_table function
Group 2 ESGI 2A3
*/

#include <stdlib.h>

#include "db.h"

Response* create_table(Query query){
    Response* res = init_response();
    Table* table = init_table();

    // check table name
    // set table name



}
/*
typedef struct 
{
    CommandType cmd_type; 

    // take one of these params based on cmd_type
    union{
        CreateParams create_params;
    } params;

    // set this if there is a syntax error that the parser detected
    char syntax_message[100];
} Query;

typedef struct{
    char table_name[TABLE_NAME_MAX];

    char **col_list; // list of column names passed in the query
    int col_count;          // number of columns

    ColType *type_list;  // list of types corresponding to column
    ColConstraintType *constraint_list; // list of constraint corresponding to order of col_list

    char** table_refer_list; //list of tables refered to by fk cols
    char** col_refer_list; // list of cols of refered tables refered to by fk cols
    int fk_count; // number of fk to free 2 lists above
} CreateParams;
*/