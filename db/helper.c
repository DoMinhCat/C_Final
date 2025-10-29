/*
Date of creation : 29/10/2025
Description : Helper functions for db core functions
Group 2 ESGI 2A3
*/

#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "../main.h"

int* get_fk_col_list_index(Query query){
    /*
    The function returns an array/pointer of index matches with col_refer_list and table_refer_list
    
    Example : 
    col_list = col1 pk, col2, col3 fk, col4, col5 fk
    fk_index = 2, 4 -> which are indexes of col3 and col5 in col_list

    Returns NULL pointer if there is error
    */
   int fk_count = query.params.create_params.fk_count;
    int* res = (int*)malloc(sizeof(int) * fk_count);
    assert(res != NULL);
    int current_index = 0;
    int i;

    switch (query.cmd_type)
    {
    case CREATE:
        int col_count = query.params.create_params.col_count;
        ColConstraintType* constraint_list = query.params.create_params.constraint_list;
        for(i=0; i<col_count; i++){
            if(constraint_list[i] == FK){
                res[current_index] = i;
                current_index++;
            }
        }
        break;
    
    default:
        free(res); // will return NULL if error
        break;
    }
    
    // handle an edge case that will probably never happens just to safe guard parse_create() if it has bug :))
    if(current_index != fk_count) free(res);

    return res;
}