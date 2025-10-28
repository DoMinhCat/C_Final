/*
Date of creation : 18/10/2025
Description : All hash-related functions/operations are here
Group 2 ESGI 2A3
*/

#include <string.h>
#include "hash.h"
#include "db.h"
#define HASH_TABLE_SIZE 67 // prime number size for better key distribution

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
unsigned int hash_all(ColType type, void* val_to_hash){
    unsigned int res;
    if(type == STRING){
        res = hash_string(*(char*)val_to_hash);
    }else{
        res = hash_int(*(int*)val_to_hash);
    }

    return res;
}
