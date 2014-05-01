btree
=====

C++ implementation of a B Tree

In order to compile this program type 'make all' into the command line.
To run the program type './btree'
There are several commands to run once you start the program:
new <order> --This creates a new tree of whatever order you specify, if a tree exists it will be deleted
insert <num> --This inserts a new key into the tree
select <num> --This searches the tree for a key, marking all nodes along the way
delete <num> --This deletes a key from the tree
getjson --This will get the JSON info for the tree
run <file> --This will run a batch file of these commands making it easy to save trees
quit --Quits the program