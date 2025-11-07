/*
Date of creation : 07/11/2025
Description : Code for SHOW operation
Group 2 ESGI 2A3
*/

#include <stdlib.h>
#include <stdio.h>

#include "db.h"
#include "../ui/parser.h"
#include "../global_var.h"

void show(Query* query){
    Table* current_table = first_table;
    int tb_count = 0;
    
    for(current_table=first_table; current_table!=NULL; current_table = current_table->next_table){
        printf("%d. %s\n", tb_count+1, current_table->name);
        tb_count++;
    }

    print_divider();
    if(tb_count>=1) printf("%d %s in total.\n", tb_count, tb_count>1?"tables":"table");
    else printf("Database is empty.\n");
}