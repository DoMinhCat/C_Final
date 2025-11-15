/*
Date of creation : 18/10/2025
Description : All hash-related functions/operations are here
Group 2 ESGI 2A3
*/

#include <string.h>
#include <assert.h>
#include <stdbool.h>

#include "hash.h"
#include "../db/db.h"
#include "../init/init.h"
#include "../db/helper_db.h"

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

Node* exist_in_ht(HashTable* hash_tab, int condition_int, char* condition_str){
    // used for WHERE clause, do a hash lookup then return the matching hash node for later processing, NULL if not found 
    int key;
    Node* current_node = NULL;
    char* val_to_cmp = NULL;

    // convert and hash int/str condition value 
    if(!condition_str) {
        val_to_cmp = int_to_str(condition_int);
        key = hash_int(condition_int);
    } else {
        val_to_cmp = strdup(condition_str);
        key = hash_string(condition_str);
    }

    // no key found
    if(!hash_tab->bucket[key]) {
        free(val_to_cmp);
        val_to_cmp = NULL;  
        return NULL;
    }

    // loop the node list that handles collision
    for(current_node=hash_tab->bucket[key]; current_node!=NULL; current_node=current_node->next_node){     
        if(strcmp(val_to_cmp,current_node->original_value)==0){
            // found a match
            free(val_to_cmp);
            val_to_cmp = NULL;  
            return current_node;
        }
    }   

    // not in collision linked list (other values hashed into the same key as this value)
    free(val_to_cmp);
    val_to_cmp = NULL;  
    return NULL;
}
