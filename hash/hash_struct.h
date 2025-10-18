/*
Date of creation : 17/10/2025
Description : All structures of hashing are delcared here
Group 2 ESGI 2A3
*/

#ifndef HASH_STRUCT_H
#define HASH_STRUCT_H
#include <stdio.h>
#include "db_struct.h"

//hash table is a list of bucketnode
typedef struct BucketNode{
    Row row;

    struct BucketNode *next_bucket_node;
} BucketNode;

typedef struct{
    unsigned int key;
    char *table_name;

    BucketNode bucket; 
} HashTable;

#endif