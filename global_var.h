#ifndef GLOBAL_VAR_H
#define GLOBAL_VAR_H

typedef struct Col Col; 
typedef struct Table Table; 
typedef struct Row Row;

// defined in global_var.c
extern Table* first_table;
extern Row* first_row;
extern Col* first_col;

#endif