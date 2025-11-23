#include <stdlib.h>

#include "global_var.h"

// global variable to be extern in db.h
Table* first_table = NULL;
Row* first_row = NULL;
Col* first_col = NULL;
int table_count = 0;

int MAX_TABLE_WIDTH = 150;
int MIN_COL_WIDTH = 15;