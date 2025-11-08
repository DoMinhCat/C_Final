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
- Operators AND and OR are not available yet
- Tables can't be altered one created, altering must be done through deleting the table, recreate it and manually reinsert all data :)
- NOT NULL constraint is not available yet
- Only one primary key is allowed for each table => relation many-many not available
- Primary key must be of type int or string, can't be negative if is int
- The order of tables passed for DROP matters (foreign key constraint)
- "exit" or "quit" to exit the program

- Max col per table = 50 TODO
- Max table = 200 TODO
- Max length for str values 257 (\0 counted) TODO
- Max chars for table/col name = 100

5. Usage

- `show tables`
- `describe table tab1`
- `delete from tab1 [where col1 = 6]`
- `drop table tab1 [, tab2, tab3,...]`
- `create table tab1 ( col1 int pk, col2 string fk references tab_to_refer col_to_refer, col3 double )`, spaces between parentheses are obligated for the program to parse the command correctly
- `insert into tab1 ( col1, col2 ) values ( val1, val2 )`
- `select ( col1, col2, col3 )/* from tab1 [join tab2 on col1 = col2] [where col1 = xyz]`

6. Contributors

Current quick start :
gcc main.c ui/parser.c ui/create.c ui/delete.c ui/drop.c ui/insert.c ui/select.c ui/show.c ui/describe.c ui/helper_ui.c clean/cmd.c clean/db.c init/query.c init/db.c init/hash_table.c db/create.c db/helper_db.c db/drop.c db/show.c db/describe.c global_var.c -o sb.exe

Commands to test:

CREATE TABLE team ( id int pk, name string unique, score double )
CREATE TABLE player ( id int pk, name string, age int, weight double, team_id int fk references team id )

Ideas:
default

NOTE for below: no need to utilise skipped gap, let next_id be the next one of that user inserted

- Auto increment for int pk (
  - add "next_id" field for struct Table
  - if insert without pk col
    - if pk not int, return err obligate value for pk
    - if pk is int: loop while infinity, check next_id is unique in pk col by hash it and look up at hash table, loop through linked list of the bucket and compare with id fields of Row of that bucket
      - if unique, set pk field = next_id, then next_id++ if pk, flag=true to break out of loop
      - if no, next_id++, then check again
  - if insert with pk col,
    - if pk is int
      - check id to insert is unique by looping infinity : hash then lookup at hash table like above
        - if no, return error
        - if unique
          - save to a var to insert later since still checking other cols and other criterias, insert operation is after all checks
          - check next_id != id to insert, if same then next_id++)
