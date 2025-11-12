/*
Date of creation : 18/10/2025
Description : All hash-related functions/operations are here
Group 2 ESGI 2A3
*/

#include <string.h>
#include "hash.h"
#include "../db/db.h"

unsigned int hash_string(char* string_to_hash){
    //DJB2 Algorithm
    unsigned int res = 5381; // prime seed (known to work well)
    unsigned char c;

    if (string_to_hash == NULL) {
        return 0; 
    }
    while ((c = *string_to_hash++)) res = ((res << 5) + res) + c; 
    return res % HASH_TABLE_SIZE;
}

unsigned int hash_int(int num_to_hash){
    unsigned int val = (unsigned int)num_to_hash;
    
    // some hash func i found on internet doing bit shift and bitwise XOR
    val = (val ^ 61) ^ (val >> 16);
    val = val + (val << 3);
    val = val ^ (val >> 4);
    
    return val % HASH_TABLE_SIZE;
}

void add_to_ht(HashTable* hash_table, int key, char* value, Row* corresponding_row){
    // function to add a node to the indicated hash table, original value can be str/int, must be converted to str before passing in this function
    Node* new_node = NULL;

    new_node = init_node();
    assert((new_node->original_value = strdup(value)) != NULL);
    new_node->row = corresponding_row;

    // set pointer for node in the linked list that handles collisions
    if(!hash_table->bucket[key]){
        hash_table->bucket[key] = new_node;
    }else{
        // insert at head
        new_node->next_node = hash_table->bucket[key];
        hash_table->bucket[key] = new_node;
    }
}