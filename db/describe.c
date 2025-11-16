/*
Date of creation : 07/11/2025
Description : Code for describe function
Group 2 ESGI 2A3
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "db.h"
#include "../ui/parser.h"
#include "helper_db.h"

void describe_table(Query* query){
    Table* table = NULL;
    Col* current_col = NULL;
    char* name = NULL;
    char* type = NULL;
    char* constraint = NULL;
    char* table_refer = NULL;
    char* col_refer = NULL;
    int i;

    // check if table exists
    table = get_table_by_name(query->params.describe_params.table_name);

    if(!table){
        fprintf(stderr, "Execution error: '%s' table not found.\n");
        return;
    } 

    printf("%-20s | %-20s | %-20s | %-25s | %-20s\n", "Column", "Type", "Constraint", "Table of Reference", "Column of Reference");
    for(i=0; i<120; i++) printf("-");
    printf("\n");

    for(current_col=table->first_col; current_col!=NULL; current_col = current_col->next_col){
        name = strdup(current_col->name);
        assert(name!=NULL);

        if(current_col->type == STRING) type = "String";
        else if(current_col->type == INT) type = "Int";
        else if(current_col->type == DOUBLE) type = "Double";
        else type = "Unknown";

        if(current_col->constraint == PK) constraint = "Primary key";
        else if(current_col->constraint == FK) constraint = "Foreign key";
        else if(current_col->constraint == UNIQUE) constraint = "Unique";
        else if(current_col->constraint == NONE) constraint = "None";
        else constraint = "Unknown";

        if(current_col->refer_table){
            table_refer = strdup(current_col->refer_table);
            assert(table_refer!=NULL);
        } else table_refer = NULL;

        if(current_col->refer_col){
            col_refer = strdup(current_col->refer_col);
            assert(col_refer!=NULL);
        } else col_refer = NULL;

        printf("%-20s | %-20s | %-20s | %-25s | %-20s\n", name, type, constraint, table_refer?table_refer:"None", col_refer?col_refer:"None");
        
        
        free(name);
        name = NULL;
        free(col_refer);
        col_refer = NULL;
        free(table_refer);
        table_refer = NULL;
    }
    for(i=0; i<120; i++) printf("-");
    printf("\n%d %s, %d %s in total.\n", table->col_count, table->col_count>1?"columns":"column", table->row_count,  table->row_count>1?"rows":"row");
}