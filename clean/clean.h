/*
Date of creation : 26/10/2025
Description : Prototypes for clean.c
Group 2 ESGI 2A3
*/

#ifndef CLEAN_H
#define CLEAN_H

typedef struct Query Query;
typedef struct Col Col;
typedef struct Row Row;
typedef struct Table Table; 
typedef struct HashTable HashTable;  
typedef struct Node Node;  

void free_current_cmd(char** cmd_string, Query** query);
void free_query(Query** query);

void free_col(Col* col);
void free_row(Row* row);
void free_table(Table* table);
void free_db(Table* first_table);
void free_hash_table(HashTable* hash_table);
void free_node(Node* node);

#endif