/*
Date of creation : 18/10/2025
Description : All hash-related functions/operations are here
Group 2 ESGI 2A3
*/

#include <stdio.h>
#include "hash.h"
#define HASH_TABLE_SIZE 67 // prime number size for better key distribution

unsigned int hash_this(int num_to_hash){
    
}

unsigned int hash_int(int num_to_hash){
    int res = 2025;
    short last_digit = num_to_hash%10;

    res = res * num_to_hash + last_digit;
    res = (res % HASH_TABLE_SIZE + HASH_TABLE_SIZE) % HASH_TABLE_SIZE; // handle negative values

    return res;
}
