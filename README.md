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
- Primary key and foreign key must be of type int or string, can't be 0 or negative if is int
- Primary key type INT auto-incrementation supported
- All foreign keys are ON DELETE RESTRICT
- The order of tables passed for DROP matters (foreign key constraint)
- "exit" or "quit" to exit the program

- Max col per table = 50
- Max length for str values 256 (\0 excluded)
- Max table = 100
- Max chars for table/col name = 100
- Row per table warning threshold = 10 000

5. Usage

- `show tables`
- `describe table tab1`
- `delete from tab1 [where col1 = 6]`
- `drop table tab1 [, tab2, tab3,...]`
- `create table tab1 ( col1 int pk, col2 string fk references tab_to_refer col_to_refer, col3 double )`, spaces between parentheses are obligated for the program to parse the command correctly
- `insert into tab1 ( col1, col2 ) values ( val1, val2 )`
- `select ( col1, col2, col3 )/* from tab1 [join tab2 on col1 = col2] [where col1 = xyz]`, order of col1 and col2 in JOIN clause matters, col1 and col2 correspond to tab1 and tab2 respectively

6. Contributors

Current quick start :
gcc main.c ui/parser.c ui/create.c ui/delete.c ui/drop.c ui/insert.c ui/select.c ui/show.c ui/describe.c ui/helper_ui.c clean/cmd.c clean/db.c init/query.c init/db.c init/hash_table.c db/create.c db/helper_db.c db/drop.c db/show.c db/insert.c db/describe.c db/where.c global_var.c hash/hash.c -o sb.exe

Commands to test:

CREATE TABLE team ( id int pk, name string unique, score double )
CREATE TABLE player ( id int pk, name string, age int, weight double, team_id int fk references team id )
DROP TABLE player // error: 'player' references 'team'
INSERT INTO team ( id, name, score ) values ( 1, abc, 45.5 )
INSERT INTO player ( team_id ) values ( 5 ) //ref id doesnt exist
INSERT INTO team ( id , name , score ) values ( 1 , duplicateteam , 100.0 ) // PK violated
INSERT INTO player ( id , name , age , weight , team_id ) values ( 99 , ronaldo , 38 , 78.0 , 1 )
INSERT INTO player ( id , name , age , weight , team_id ) values ( 15 , stringfk , 22 , 70.0 , abc ) // invalid FK

Ideas:
default constraint
end with ; to be able to execute multiple cmds in one run
where x = NULL

SELECT without where: print on the go
SELECT with WHERE (no indexing) : print on the go with a compare to check condition (like above + check)
SELECT with WHERE (indexing) : look up hash table, return list of Row*, loop through result rows and print on the go

