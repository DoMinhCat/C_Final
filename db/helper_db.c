/*
Date of creation : 29/10/2025
Description : Helper functions for db core functions
Group 2 ESGI 2A3
*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <float.h>
#include <math.h>

#include "helper_db.h"
#include "../ui/parser.h"
#include "db.h"
#include "../global_var.h"
#include "../hash/hash.h"

bool table_exists(char* table_name){
    // check for table existence, print error if not.
    // ex: if(!table_exists(tb_name)) return;
    Table* table = get_table_by_name(table_name);
    if(!table) {
        fprintf(stderr, "Execution error: '%s' table not found.\n", table_name);
        return false;
    }
    return true;
}

bool col_exists(Table* table, char* col_name){
    // same as table_exists above but for col
    Col* col = get_col_by_name(table, col_name);
    if(!col) {
        fprintf(stderr, "Execution error: '%s' column  not found.\n", col_name);
        return false;
    }
    return true;
}

int* get_fk_col_list_index(Query* query){
    /*
    The function returns an array/pointer of index matches with col_refer_list and table_refer_list
    Use this to convert col_refer_list and table_refer_list index to col_list, type_list, or constraint_list

    Example : 
    col_list = col1 pk, col2, col3 fk, col4, col5 fk
    fk_index = 2, 4 -> which are indexes of col3 and col5 in col_list

    Returns NULL pointer if there is error
    */
    int fk_count = query->params.create_params.fk_count;
    int* res = (int*)malloc(sizeof(int) * fk_count);
    assert(res != NULL);
    int current_index = 0;
    int i;

    switch (query->cmd_type)
    {
    case CREATE:
        int col_count = query->params.create_params.col_count;
        ColConstraintType* constraint_list = query->params.create_params.constraint_list;
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

Table* get_last_table(Table* first_table){
    // this function assumes there are at least 1 table already
    Table* current_table = first_table;
    
    while(current_table->next_table){
        current_table = current_table->next_table;
    }
    return current_table;
}

// NOTE: DON'T DELETE, maybe will need this in the future for delete/where/select?
// Col* get_last_col(Col* first_col){
//     // this function assumes there are at least 1 col already
//     Col* current_col = first_col;
    
//     while(current_col->next_col){
//         current_col = current_col->next_col;
//     }
//     return current_col;
// }

// HashTable* get_last_hash_table(HashTable* first_ht){
//     // this function assumes there are at least 1 hash table already
//     HashTable* current_ht = first_ht;
    
//     while(current_ht->next_hash_table){
//         current_ht = current_ht->next_hash_table;
//     }
//     return current_ht;
// }
Row* get_last_row(Row* first_row){
    // this function assumes there are at least 1 row already
    Row* current_row = first_row;
    
    while(current_row->next_row){
        current_row = current_row->next_row;
    }
    return current_row;
}

Table* get_table_by_name(char* table_name) {
    // this func return pointer to the table having input name
    Table* current = first_table;

    while (current != NULL) {
        if (strcmp(current->name, table_name) == 0) {
            return current;
        }
        current = current->next_table;
    }
    return NULL;
}

Col* get_col_by_name(Table* table, char* col_name) {
    // return pointer to column with input name of a given table
    Col* current = table->first_col;
    
    while (current != NULL) {
        if (strcmp(current->name, col_name) == 0) return current;
        current = current->next_col;
    }
    return NULL;
}

HashTable* get_ht_by_col_name(HashTable* first_ht, char* col_name){
    while(first_ht!=NULL){
        if(strcmp(first_ht->col_name, col_name)==0) return first_ht;

        first_ht = first_ht->next_hash_table;
    }
    return NULL;
}

int get_data_list_index(Table* table, char* col_name){
    // get the index of data list of Row for the corresponding type list, use this to access to data field of row (same as SELECT col1) or to insert into the right place of the list of row struct
    /* ex: 
    col1 int, col2 str, col3 str, col4 int
        0                            1
    get_data_list_index(table, "col4") -> 1 (col4 is INT so col4 is has index 1 for INT col)
    */
    Col* current = table->first_col;
    int i_int = -1;
    int i_str = -1;
    int i_double = -1;
    
    while (current != NULL) {
        if(current->type == INT) i_int++;
        else if(current->type == DOUBLE) i_double++;
        else if(current->type == STRING) i_str++;

        if (strcmp(current->name, col_name) == 0) {
            switch(current->type) {
            case INT:
                return i_int;
                break;
            case STRING:
                return i_str;
                break;
            case DOUBLE:
                return i_double;
                break;
            default:
                return -1; // not gonna happen tho, all col type is INT/STR/DOUBLE
                break;
            }
        }
        current = current->next_col;
    }
    return -1; // if col not found
}

/*
int compare_double(double val1, double val2){
    //safely compare double
    // return 0 if val1=val2
    // return 1 if val1>val2
    // return -1 if val1<val2
    double epsilon = DBL_EPSILON * 10.0;

    if (fabs(val1 - val2) <= epsilon) return 0;
    else if(val1<val2) return -1;
    else return 1;
}*/

bool refer_val_exists(char* str_to_check, int val_to_check, char* ref_table_name, char* ref_col_name){
    // this func check if the inserted value for fk exists in the referenced column of the referenced table
    int sscanf_check = 0;
    int hashed_int;
    Node* current_hash_node = NULL;
    HashTable* hash_tab = get_ht_by_col_name(get_table_by_name(ref_table_name)->first_hash_table, ref_col_name); 

    // no need to check if referenced table exists: this is restricted by drop (can't drop table if it is still refernced)

    // str value check
    if(str_to_check!=NULL){
        //hash and check existenece with hash table
        hashed_int = hash_string(str_to_check); // this is the key 0-66

        // bucket null => value non existing, ref integrity violated 
        if(hash_tab->bucket[hashed_int] == NULL){
            fprintf(stderr, "Execution error: referential integrity violated. Value '%s' for '%s' column of '%s' table does not exist.\n", str_to_check, ref_col_name, ref_table_name);
            return false;
        }
        
        for(current_hash_node = hash_tab->bucket[hashed_int]; current_hash_node!=NULL; current_hash_node=current_hash_node->next_node){
            if(strcmp(current_hash_node->original_value, str_to_check) == 0) return true;
        }
        
        //no result found
        fprintf(stderr, "Execution error: referential integrity violated. Value '%s' for '%s' column of '%s' table does not exist.\n", str_to_check, ref_col_name, ref_table_name);
        return false;

    // int value check
    }else{
        //hash and check existenece with hash table
        hashed_int = hash_int(val_to_check); // this is the key 0-66
        int val_db;

        // bucket null => no duplicate value, no need to check 
        if(hash_tab->bucket[hashed_int] == NULL){
            fprintf(stderr, "Execution error: referential integrity violated. Value '%d' for '%s' column of '%s' table does not exist.", val_to_check, ref_col_name, ref_table_name);
            return false;
        }

        for(current_hash_node = hash_tab->bucket[hashed_int]; current_hash_node!=NULL; current_hash_node=current_hash_node->next_node){
            // convert back to int to compare, no need strol because we are sure it is int converted to string when inserted and passed earlier checks
            sscanf_check = sscanf(current_hash_node->original_value, "%d", &val_db); 
            if(sscanf_check != 1){
                fprintf(stderr, "Execution error: an error occured while hashing.\n");
                return false;
            }
            if(val_db == val_to_check) return true;
        }
        
        //no result found
        fprintf(stderr, "Execution error: referential integrity violated. Value '%d' for '%s' column of '%s' table does not exist.", val_to_check, ref_col_name, ref_table_name);
        return false;
    }
}

bool pk_value_is_unique(char* str_to_check, int val_to_check, HashTable* hash_tab){
    // this func check uniqueness with hash table lookup, use for pk and unique cols
    int sscanf_check = 0;
    int hashed_int;
    Node* current_hash_node = NULL;

    // str value
    if(str_to_check!=NULL){
        //hash and check uniqueness with hash table
        hashed_int = hash_string(str_to_check); // this is the key 0-66

        // bucket null => no duplicate value, no need to check 
        if(hash_tab->bucket[hashed_int] != NULL){
            for(current_hash_node = hash_tab->bucket[hashed_int]; current_hash_node!=NULL; current_hash_node=current_hash_node->next_node){
                if(strcmp(current_hash_node->original_value, str_to_check) == 0){
                    fprintf(stderr, "Execution error: PRIMARY KEY constraint violated.\n");
                    return false;
                }
            }
        }
        //all checks passed
        return true;
    
    // int value
    }else{
        // 0 and negative not allowed
        if(val_to_check<=0){
            fprintf(stderr, "Execution error: values with PRIMARY KEY constraint must be 1 or larger.\n");
            return false;
        }
        //hash and check uniqueness with hash table
        hashed_int = hash_int(val_to_check); // this is the key 0-66
        int val_db;

        // bucket null => no duplicate value, no need to check 
        if(hash_tab->bucket[hashed_int] != NULL){
            for(current_hash_node = hash_tab->bucket[hashed_int]; current_hash_node!=NULL; current_hash_node=current_hash_node->next_node){
                // convert back to int before cmp, no need strol because we are sure it is int converted to string when inserted and passed earlier checks
                sscanf_check = sscanf(current_hash_node->original_value, "%d", &val_db); 
                if(sscanf_check != 1){
                    fprintf(stderr, "Execution error: an error occured while hashing.\n");
                    return false;
                }
                if(val_db == val_to_check){
                    fprintf(stderr, "Execution error: PRIMARY KEY constraint violated.\n");
                    return false;
                }
            }
        }
        //all checks passed
        return true;
    }
}

char* int_to_str(int val){
    char buffer[20];
    char* res = NULL;

    snprintf(buffer, 20, "%d", val);
    assert((res = strdup(buffer))!=NULL);
    return res;
}