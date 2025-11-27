/*
Date of creation : 16/11/2025
Description : Code for database export to file 
Group 2 ESGI 2A3
*/

#include <stdio.h>
#include <stdbool.h>

#include "../db/db.h"
#include "../hash/hash.h"
#include "../global_var.h"

/*
IMPORTANT: for all strings read later need to alloc strlen + 1 and set \0 at the end

NOTE: export structure for table to fwrite in order:
    # metadata:
    strlen(tab_name) + 1
    Table name

    Num of Cols
    Num of Rows
    Num of Hash Tables

    Next_id of table (int)
    Col one by one:
        name
        type
        constraint
        strlen of refer_table
        refer_table (str)   
        strlen of refer_col
        refer_col (str)

    Row one by one:
        int_count
        double_count
        str_count

        IMPORTANT: before each item is a char 1 or 0 to indicate if it is null or not
        example: 0|1value

        int items: null marker then item
        double items: null marker then item
        str items: null marker, string len + 1 then item

    Hash table one by one:
        strlen of col_name, col_name

        loop 67 times for each bucket:
            Loop and write num of nodes that handle collision
            for prev_row and row, write the row position (int), not the pointer
            strlen of original_value and original_value (str)

*/ 

bool write_succeed(int written, int count, char* file_name){
    if(written != count){
        fprintf(stderr, "Export error: writing to '%s' failed.\n\n");
        return false;
    }
    return true;
}

bool export_col(FILE* output_file, char* output_file_name, Col* col){
    int written;
    int len_name = strlen(col->name) + 1;
    int len_ref_tab = strlen(col->refer_table)+1;
    int len_ref_col = strlen(col->refer_col)+1;
    
    // len of col name
    written = fwrite(&len_name, sizeof(int), 1, output_file);
    if(!write_succeed(written, 1, output_file_name)) return false;
    // col name
    written = fwrite(col->name, sizeof(char), len_name, output_file);
    if(!write_succeed(written, len_name, output_file_name)) return false;

    // type
    written = fwrite(&col->type, sizeof(ColType), 1, output_file);
    if(!write_succeed(written, 1, output_file_name)) return false;
    // constraint
    written = fwrite(&col->constraint, sizeof(ColConstraintType), 1, output_file);
    if(!write_succeed(written, 1, output_file_name)) return false;

    // len of refer table
    written = fwrite(&len_ref_tab, sizeof(int), 1, output_file);
    if(!write_succeed(written, 1, output_file_name)) return false;
    // refer tab
    written = fwrite(col->refer_table, sizeof(char), len_ref_tab, output_file);
    if(!write_succeed(written, len_ref_tab, output_file_name)) return false;

    // len of refer col
    written = fwrite(&len_ref_col, sizeof(int), 1, output_file);
    if(!write_succeed(written, 1, output_file_name)) return false;
    // refer col
    written = fwrite(col->refer_col, sizeof(char), len_ref_col, output_file);
    if(!write_succeed(written, len_ref_col, output_file_name)) return false;    

    return true;
}

bool export_row(FILE* output_file, char* output_file_name, Row* row){
    int written;
    int i;
    int len_str;
    unsigned char null_marker;

    // int count
    written = fwrite(&row->int_count, sizeof(int), 1, output_file);
    if(!write_succeed(written, 1, output_file)) return false;
    // double count
    written = fwrite(&row->double_count, sizeof(int), 1, output_file);
    if(!write_succeed(written, 1, output_file)) return false;
    // str count
    written = fwrite(&row->str_count, sizeof(int), 1, output_file);
    if(!write_succeed(written, 1, output_file)) return false;

    // int list
    for(i=0; i<row->int_count; i++){
        // null marker 
        null_marker = row->int_list[i] ? 1 : 0;
        written = fwrite(&null_marker, sizeof(unsigned char), 1, output_file);
        if(!write_succeed(written, 1, output_file)) return false;
        // write value if not null
        if(row->int_list[i]){
            written = fwrite(row->int_list[i], sizeof(int), 1, output_file);
            if(!write_succeed(written, 1, output_file)) return false;
        }
    }

    // double list
    for(i=0; i<row->double_count; i++){
        // null marker 
        null_marker = row->double_list[i] ? 1 : 0;
        written = fwrite(&null_marker, sizeof(unsigned char), 1, output_file);
        if(!write_succeed(written, 1, output_file)) return false;
        // write value if not null
        if(row->double_list[i]){
            written = fwrite(row->double_list[i], sizeof(double), 1, output_file);
            if(!write_succeed(written, 1, output_file)) return false;
        }
    }

    // string list
    for(i=0; i<row->str_count; i++){
        // null marker 
        null_marker = row->str_list[i] ? 1 : 0;
        written = fwrite(&null_marker, sizeof(unsigned char), 1, output_file);
        if(!write_succeed(written, 1, output_file)) return false;
        
        // write if not null
        if(row->str_list[i]){
            len_str = strlen(row->str_list[i]) + 1;
            // len of str item
            written = fwrite(&len_str, sizeof(int), 1, output_file);
            if(!write_succeed(written, 1, output_file)) return false;
            // string value
            written = fwrite(row->str_list[i], sizeof(char), 1, output_file);
            if(!write_succeed(written, 1, output_file)) return false;
        }
    }
    return true;
}

bool export_hash_node(FILE* output_file, Node* hash_node){
    // TODO
}

bool export_hash_table(FILE* output_file, HashTable* ht_to_export){
    // TODO
}

bool export_table(FILE* output_file, char* output_file_name, Table* table){
    int written;
    int tab_name_len = strlen(table->name) + 1;
    int i;
    Col* current_col = NULL;
    Row* current_row = NULL;
    HashTable* current_ht = NULL;

    // write length of table name + 1 (for \0)
    written = fwrite(&tab_name_len, sizeof(int), 1, output_file);
    if(!write_succeed(written, 1, output_file_name)) return false;
    // name of table
    written = fwrite(table->name, sizeof(char), tab_name_len, output_file);
    if(!write_succeed(written, tab_name_len, output_file_name)) return false;

    // num of cols
    written = fwrite(&(table->col_count), sizeof(int), 1, output_file);
    if(!write_succeed(written, 1, output_file_name)) return false;
    // num of rows
    written = fwrite(&(table->row_count), sizeof(int), 1, output_file);
    if(!write_succeed(written, 1, output_file_name)) return false;
    // num of hash tables
    written = fwrite(&(table->hash_table_count), sizeof(int), 1, output_file);
    if(!write_succeed(written, 1, output_file_name)) return false;

    // next_id
    written = fwrite(&(table->next_id), sizeof(int), 1, output_file);
    if(!write_succeed(written, 1, output_file_name)) return false;

    // write columns
    current_col = table->first_col;
    for(i=0; i<table->col_count; i++){
        if(!export_col(output_file, output_file_name, current_col)) return false;
        current_col=current_col->next_col;
    }

    // write rows
    current_row = table->first_row;
    for(i=0; i<table->row_count; i++){
        if(!export_row(output_file, output_file_name, current_row)) return false;
        current_row=current_row->next_row;
    }

    // write hash tables
    current_ht = table->first_hash_table;
    for(i=0; i<table->hash_table_count; i++){
        if(!export_hash_table()) return false;
        current_ht=current_ht->next_hash_table;
    }
    


    return true;
}

void export_db(char* output_file_name, Table* first_table){
    // export the whole database to a binary file

    FILE* output_file = NULL;
    Table* current_table = NULL;
    char* extension = ".bin";
    int i;
    int written;

    //add the .bin extension
    output_file_name = realloc(output_file_name, strlen(output_file_name) + strlen(extension) + 1);
    assert(output_file_name!=NULL);
    strcat(output_file_name, extension);

    // open file to write
    output_file = fopen(output_file_name, "wb");
    if(!output_file){
        fprintf(stderr, "Export error: failed to create '%s'.\n\n", output_file_name);
        return;
    }

    // write number of tables in db
    written = fwrite(&table_count, sizeof(int), 1, output_file);
    if(!write_succeed(written, 1, output_file_name)) return;
    
    // write table one by one
    current_table = first_table;
    for(i=0; i<table_count; i++){
        if(!export_table(output_file, output_file_name, current_table)){
            fclose(output_file);
            return;
        }
        current_table = current_table->next_table;
    }
}