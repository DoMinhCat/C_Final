/*
Date of creation : 17/10/2025
Description : All structures of hashing are delcared here
Group 2 ESGI 2A3
*/

#ifndef HASH_H
#define HASH_H
#define HASH_TABLE_SIZE 67 // prime number size for better key distribution

#include "../main.h"
typedef struct Row Row;


//linked list of nodes in a bucket
typedef struct Node{
    Row* row; //what row is store at this node

    // the actual value before hashed/value of pk, we can access to it by accessing to row above but will be complicated, so store it here for fast access
    char* original_value; // actual value can be int/str but converted upon inserting, need to convert back to original type to cmp

    struct Node* next_node; 
} Node;

typedef struct HashTable{
    char* pk_col_name; // name of pk col to hash

    Node* bucket[HASH_TABLE_SIZE]; // linked list of buckets, 67 buckets max -> bucket[67][linkedlist collision]
} HashTable;


unsigned int hash_int(int);
unsigned int hash_string(char*);

#endif