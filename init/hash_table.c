/*
Date of creation : 29/10/2025
Description : Clean initialisation of Hash table
Group 2 ESGI 2A3
*/

#include "../hash/hash.h"

#include <stdlib.h>
#include <assert.h>

Node* init_node(){
    Node* node = NULL;
    assert((node = (Node*)malloc(sizeof(Node))) != NULL);

    node->row = NULL;
    node->key = 0;
    node->next_node = NULL;
    return node;
}

HashTable* init_hash_table(){
    HashTable* hash_tab = NULL;
    assert((hash_tab = (HashTable*)malloc(sizeof(HashTable))) != NULL);
    
    hash_tab->table_index = 0;
    assert((hash_tab->bucket = (Node**)malloc(sizeof(Node*)*HASH_TABLE_SIZE)) != NULL);

    // init each bucket
    for(int i=0; i<HASH_TABLE_SIZE; i++){
        hash_tab->bucket[i] = init_node();
        hash_tab->bucket[i]->key = i;
    }
    return hash_tab;
}

