/*
Date of creation : 13/11/2025
Description : Code for WHERE clause with hash look up/traverse
Group 2 ESGI 2A3
*/

#include <stdlib.h>
#include <stdbool.h>

#include "db.h"
#include "helper_db.h"
#include "../hash/hash.h"
#include "../init/init.h"

FilteredRow* hash_lookup(HashTable* hash_table, int condition_int, char* condition_str){
    // perform a hash look up to the hash table of pk/unique column and return linked list of FilteredRow (but 1 item only because hash table is for unique/pk col) for WHERE clause in SELECT

    FilteredRow* result = init_filtered_row();
    // do hash lookup and get the matching hash node
    Node* hash_node = exist_in_ht(hash_table, condition_int, condition_str);

    if(hash_node){
        result->row = hash_node->row;
        return result;
    }
    return NULL;
}

FilteredRow* where_for_select(Table* table, char* condition_col_name, char* str_condition, int int_condition){
    FilteredRow* res = NULL;
    HashTable* ht_of_col = NULL;
    Col* current_col = NULL;

    bool col_is_indexed = false;

    //check if condition col is indexed
    for(ht_of_col = table->first_hash_table; ht_of_col!=NULL; ht_of_col = ht_of_col->next_hash_table){
        if(strcmp(ht_of_col->col_name, condition_col_name) == 0){
            col_is_indexed = true;
            break;
        }
    }

    if(col_is_indexed){
        res = hash_lookup(ht_of_col, int_condition, str_condition);
        return res;        
    }else{

    }

}