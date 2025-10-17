/*
Date of creation : 17/10/2025
Description : Universal structures
Group 2 ESGI 2A3
*/
#ifndef FUNC_H
#define FUNC_H
#include <stdio.h>

typedef enum{
    CREATE,
    INSERT,
    DELETE,
    SELECT,
    DROP,
    EXIT
} CommandType;
typedef struct 
{
    CommandType cmd_type; 

    // For Create table command 
    char table_name[31]; //max length = 30 chars
    

    // Error message to set if there is syntax error or parameter excede max num of chars allowed
    char err_msg[100];
} Query;

#endif
