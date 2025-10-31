/*
Date of creation : 18/10/2025
Description : All hash-related functions/operations are here
Group 2 ESGI 2A3
*/

#include <string.h>
#include <stdlib.h>

#include "hash.h"
#include "db.h"

unsigned int hash_string(char* string_to_hash){
    unsigned int res = 0;
    int length = strlen(string_to_hash);
    int i;

    for(i=0; i<length; i++){
        res = res + string_to_hash[i];
        res = res * string_to_hash[i%10];
        res = res % HASH_TABLE_SIZE;
    }
    return res;
}

unsigned int hash_int(int num_to_hash){
    unsigned int res = 2025;
    short last_digit = num_to_hash%10;

    res = res * num_to_hash + last_digit;
    res = (res % HASH_TABLE_SIZE + HASH_TABLE_SIZE) % HASH_TABLE_SIZE; // handle negative values

    return res;
}

// TODO : review input type when implement Insert function
// need to know col index in linked list to match with data_field index of Row to hash
int hash_pk(Col* first_col, Row* row_to_hash, int pk_col_index){
    ColType type;
    Col* current_col = first_col;

    int res;
    int i;
    
    // to do in insert : loop to get index and coltype of pk in col list, based on that index get the correct data in data_field of Row to hash
    /*
    for(current_col=table.first_col; current_col != NULL; current_col=current_col->next_col){
        if(current_col->constraint == PK){
            type = current_col->type;
            break;
        }
        index++;
    }
        */

       
       
    // get the type pk_col from index given
    for(i=0; i<pk_col_index; i++) current_col=current_col->next_col;
    type = current_col->type;

    

    // hash the value based on its type
    if(type == STRING){
        res = hash_string((char*)row_to_hash->data_field[pk_col_index]);
    }else if(type == INT){
        res = hash_int(atoi(row_to_hash->data_field[pk_col_index]));  // atoi converts string -> int
    }else return -1; // safe guard if it is DOUBLE, although checked in insert()

    return res;
}
