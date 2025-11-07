/*
Date of creation : 17/10/2025
Description : common types declaration
Group 2 ESGI 2A3
*/

#ifndef MAIN_H
#define MAIN_H

void print_divider();
void flush_extra();
int main(int argc, char ** argv);

typedef enum ColType{
    INT,
    DOUBLE,
    STRING    
} ColType;

typedef enum{
    NONE,
    FK,
    PK
} ColConstraintType;

#endif
