/*
Date of creation : 17/10/2025
Description : Code for JOIN operation with bubble sort and linear search
Group 2 ESGI 2A3
*/

#include <stdlib.h>

#include "db.h"

FilteredRow* bubble_sort(Table* table){
    // sort a list of rows based on joined column 
    // TODO: bubble sort with double loop on filtered struct passed from join(), need to manage current/prev/next pointer
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
    TODO: copy linked list of 2 tables to 2 filtered struct
    bubble_sort 2 sorted filtered struct
    merge_sorted_lists into 1 final filtered struct
    return that struct
    */ 
}