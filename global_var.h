#ifndef GLOBAL_VAR_H
#define GLOBAL_VAR_H

typedef struct Col Col; 
typedef struct Table Table; 
typedef struct Row Row;

// defined in global_var.c
extern Table* first_table;
extern int table_count;

extern int MAX_TABLE_WIDTH;
extern int MIN_COL_WIDTH;
#endif