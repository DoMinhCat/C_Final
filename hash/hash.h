/*
Date of creation : 17/10/2025
Description : All structures of hashing are delcared here
Group 2 ESGI 2A3
*/

#ifndef HASH_H
#define HASH_H
#define HASH_TABLE_SIZE 67 // prime number size for better key distribution
#include "../db/db.h"

//linked list of nodes in a bucket
typedef struct Node{
    Row* row; //what row is store at this node
    unsigned int key; // from 0 to 66

    struct Node* next_node; 
} Node;

typedef struct{
    int table_index; //hash table of which table in the linked list

    Node** bucket; // linked list of buckets, 67 buckets max -> bucket[67][linkedlist collision]
} HashTable;


unsigned int hash_int(int);
unsigned int hash_string(char*);
unsigned int hash_all(ColType, void*);

#endif