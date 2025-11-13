/*
Date of creation : 13/11/2025
Description : Code for WHERE clause with hash look up/traverse
Group 2 ESGI 2A3
*/

#include <stdlib.h>

#include "db.h"
#include "../hash/hash.h"

Row** hash_lookup(HashTable* hash_table, int condition_int, char* condition_str){
    // perform a hash look up to the hash table of pk/unique column and return all the Rows* that satisfies the condition

    int key;
    int size = 0;
    Row** result = NULL;
    Node* current_node = NULL;
    char* val_to_cmp = NULL;

    if(!condition_str) {
        val_to_cmp = int_to_str(condition_int);
        key = hash_int(condition_int);
    } else {
        val_to_cmp = strdup(condition_str);
        key = hash_string(condition_str);
    }

    // no key inserted
    if(!hash_table->bucket[key]) {
        free(val_to_cmp);
        val_to_cmp = NULL;  
        return NULL;
    }

    // loop the node list that handles collision
    for(current_node=hash_table->bucket[key]; current_node!=NULL; current_node=current_node->next_node){     
        if(strcmp(val_to_cmp,current_node->original_value)==0){
            assert((result = (Row**)realloc(result, sizeof(Row*) * (size+1)))!=NULL);
            result[size] = current_node->row;
            size++;
        }
    }
    
    if(size==0){
        free(val_to_cmp);
        val_to_cmp = NULL;
        return NULL;
    }
    // terminate with NULL
    assert((result = (Row**)realloc(result, sizeof(Row*) * (size+1)))!=NULL);
    result[size] = NULL;
    return result;
}

// TODO : traverse rows in case no indexing