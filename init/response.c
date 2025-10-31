/*
Date of creation : 28/10/2025
Description : Clean initialisation of Response structure
Group 2 ESGI 2A3
*/

#include <stdlib.h>
#include <assert.h>

#include "init.h"
#include "../ui/parser.h"

// safely initialise Query struct
Response* init_response(){
    Response* res = NULL;

    assert((res = (Response*)malloc(sizeof(Response))) != NULL);
    res->status = FAILURE;
    res->message[0] = '\0';

    return res;
}