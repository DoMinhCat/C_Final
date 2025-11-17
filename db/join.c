/*
Date of creation : 17/11/2025
Description : Code for JOIN operation with bubble sort and linear search
Group 2 ESGI 2A3
*/

#include <stdlib.h>
#include <stdbool.h>

#include "db.h"
#include "../init/init.h"
#include "helper_db.h"

FilteredRow* bubble_sort(FilteredRow* head_list, int row_count1, Table* join_table, Col* join_col){
    // sort a list of rows based on joined column 
    // TODO: bubble sort with double loop on filtered struct passed from join(), need to manage current/prev/next pointer

    bool swap_done;
    bool should_swap;
    int i,j;

    ColType col_type = join_col->type;
    FilteredRow* current_filtered_row = NULL;
    Row* actual_row = NULL;
    Row* next_actual_row = NULL;
    int** int_list_to_cmp = NULL;
    int** next_int_list_to_cmp = NULL;
    int* int_data_to_cmp = NULL;
    int* next_int_data_to_cmp = NULL;

    double** double_list_to_cmp = NULL;
    double** next_double_list_to_cmp = NULL;
    double* double_data_to_cmp = NULL;
    double* next_double_data_to_cmp = NULL;

    char** str_list_to_cmp = NULL;
    char** next_str_list_to_cmp = NULL;
    char* str_data_to_cmp = NULL;
    char* next_str_data_to_cmp = NULL;

    int data_index;

    // bubble sort
    for(i=0; i<row_count1-1; i++){
        swap_done = false;
        current_filtered_row = head_list;

        for(j=0; j<row_count1-i-1; j++){ // largest item at the end, no need to compare again items sorted at the back
            actual_row = current_filtered_row->row;
            next_actual_row = current_filtered_row->next_filtered_row->row;
            should_swap = false;
            data_index = get_data_list_index(join_table, join_col->name);

            // get right data field to compare and compare them
            // IMPORTANT: handle NULL case, NULL will be placed before NOT NULL values so: NULL, NULL, 1, 3, 4,...
            switch (col_type) { 
            case INT:
                // get right data 
                int_list_to_cmp = (int**)actual_row->int_list;
                int_data_to_cmp = (int*)int_list_to_cmp[data_index];
                next_int_list_to_cmp = (int**)next_actual_row->int_list;
                next_int_data_to_cmp = (int*)next_int_list_to_cmp[data_index];

                // compare
                if(int_data_to_cmp[0] > next_int_data_to_cmp[0]) should_swap = true;
                break;
            
            case DOUBLE:
                double_list_to_cmp = (double**)actual_row->double_list;
                double_data_to_cmp = (double*)double_list_to_cmp[data_index];
                next_double_list_to_cmp = (double**)next_actual_row->double_list;
                next_double_data_to_cmp = (double*)next_double_list_to_cmp[data_index];
                
                // compare double
                if(compare_double(double_data_to_cmp[0], next_double_data_to_cmp[0]) == 1) should_swap = true;
                break;

            case STRING:
                str_list_to_cmp = (char**)actual_row->str_list;
                str_data_to_cmp = (char*)str_list_to_cmp[data_index];
                next_str_list_to_cmp = (char**)next_actual_row->str_list;
                next_str_data_to_cmp = (char*)next_str_list_to_cmp[data_index];
                
                // compare string a-z order
                if(strcmp(str_data_to_cmp, next_str_data_to_cmp) > 0) should_swap = true;
                break;
            default:
                break;
            }
            
            // swap if not in right order
            if(should_swap){
                // TODO swap pointers

                swap_done = true;
            }

            current_filtered_row = current_filtered_row->next_filtered_row;
        }
        // stop sorting if no swapping done
        if(!swap_done) break;
    }

}

FilteredRow* merge_sorted_lists(FilteredRow* list1, FilteredRow* list2){
    /*
    TODO: 2 pointers on each struct
    compare joined col value of 2 pointers 
        if == : merge data fields and save to result struct 
        if != : the smaller one advance then compare again
        */  
}

FilteredRow* join(Table* tab1, Table* tab2, Col* col1, Col* col2){
    // join the 2 tables, return filtered rows for final result print of select

    /*
    TODO: copy row linked list of 2 tables to 2 filtered struct: ok
    bubble_sort 2 sorted filtered struct
    merge_sorted_lists into 1 final filtered struct
    return that struct
    */ 
   FilteredRow* head_list1 = NULL;
   FilteredRow* head_list2 = NULL;
   FilteredRow* new_node = NULL;
   Row* current_row = NULL;

   int row_count1 = tab1->row_count;
   int row_count2 = tab2->row_count;

   // copy linked list Row of tab1
   for(current_row = tab1->first_row; current_row; current_row=current_row->next_row){
        new_node = init_filtered_row();
        new_node->row = current_row;
        add_filtered_row(head_list1, new_node);
   }
   //copy linked list Row of tab2
    for(current_row = tab2->first_row; current_row; current_row=current_row->next_row){
        new_node = init_filtered_row();
        new_node->row = current_row;
        add_filtered_row(head_list2, new_node);
    }

}