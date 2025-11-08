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
    unsigned int key; // from 0 to 66

    struct Node* next_node; 
} Node;

typedef struct HashTable{
    char* pk_col_name; // name of pk col to hash

    Node* bucket[HASH_TABLE_SIZE]; // linked list of buckets, 67 buckets max -> bucket[67][linkedlist collision]
} HashTable;


unsigned int hash_int(int);
unsigned int hash_string(char*);

#endif