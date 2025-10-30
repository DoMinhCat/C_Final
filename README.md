# C_Final

Projet final de C à ESGI

## Table of contents

1. Project Description
2. Pre-requisites
3. Installation
4. Features

- Strings are applied directly, no need to wrap them in quotes or double quotes
- Table and column names are case sensitive
- Available types : int, string, and double
- Insert: values will be inserted in the order of which columns are created and all columns must be inserted (can't insert only 4 values into a table with 5 columns)
- The primary column value will not be automatically generated or incremented, must be manually inserted with INSERT INTO
- All command ends with a semicolon without spaces before it
- Operators AND and OR are not available
- Tables can't be altered one created, altering must be done through deleting the table, recreate it and manually reinsert all data :)
- The column with foreign key attribute and the column it points to must have the same name in order for JOIN operation to work
- JOIN and SELECT can't be used in the same SELECT command
- NOT NULL and UNIQUE constraint are not available
- Only one primary key is allowed for each table => relation many-many not available
- A semicolon (;) at the end of a command is not allowed since it is command line based
- Primary key must be of type int or string
- "exit" or "quit" to exit the program

- Max col per table = 50
- Max table = 50
- Max chars for table/col name = 100

5. Usage

- `delete from tab1 where col1 = 6`
- `drop table tab1`
- `create table tab1 ( col1 int pk, col2 string fk references tab_to_refer col_to_refer, col3 double )`, spaces between parentheses are obligated for the program to parse the command correctly
- `insert into tab1 ( col1, col2 ) values ( val1, val2 )`

6. Contributors

Quick start Cat: gcc sandbox.c ui/create.c ui/delete.c ui/drop.c ui/insert.c ui/parser.c ui/select.c clean/clean.c init/query.c -o sandbox.exe

Debug includes: gcc main.c ui/parser.c ui/create.c ui/delete.c ui/drop.c ui/insert.c ui/select.c clean/clean.c init/query.c db/create.c -o sb.exe
