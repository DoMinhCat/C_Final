/*
Date of creation : 08/11/2025
Description : Code of INSERT statement
Group 2 ESGI 2A3
*/

#include <stdlib.h>

#include "db.h"
#include "../ui/parser.h"

void insert(Query* query){
    char** col_list = query->params.insert_params.col_list;
    int col_count = query->params.insert_params.col_count;
    void** data_list = query->params.insert_params.data_list;
    int tb_name = query->params.insert_params.table_name;


    // check table exist
    // check col_list all exist
    // validate col type/value type: int/string and double/string
}