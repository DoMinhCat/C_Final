/*
Date of creation : 26/10/2025
Description : prototypes for all init functions
Group 2 ESGI 2A3
*/

#ifndef INIT_H
#define INIT_H

typedef struct Node Node;  
typedef struct HashTable HashTable; 
typedef struct Table Table;  
typedef struct Query Query;  
typedef struct Response Response; 
typedef struct Col Col;  
typedef struct Row Row;  

Query* init_query();
Response* init_response();

Node* init_node();
HashTable* init_hash_table();

Table* init_table();
Col* init_col();
Row* init_row();

#endif