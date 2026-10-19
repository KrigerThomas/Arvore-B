# B-Tree Index in C

B-tree index written in C for the Data Structures II course at Universidade Vila Velha (UVV).

The program reads a text file of records (id, name and phone), indexes each id together with the byte offset of its line in the file, and then lets you add new records, search by id and save the tree to an index file. A search walks the tree to find the offset and jumps straight to the record with fseek, without reading the whole file.

## Two insertion strategies

| Folder | How splits happen | Order |
|---|---|---|
| bottom-up split | The key goes down to a leaf and full nodes are split on the way back up, promoting the middle key to the parent | 3 |
| preemtive split | Full nodes are split on the way down, before the key reaches the leaf (preemptive or top-down split) | 6 |

Each folder has its own main.c, arvoreB.c and arvoreB.h, plus sample source files (fonteN.txt) and the index files they generated (indiceN.txt).

## How to run

Inside one of the folders:

    gcc main.c arvoreB.c -o arvoreB
    ./arvoreB fonte4.txt indice4.txt

The first argument is the source file with the records and the second is the index file that option 3 writes. Each line of the source file looks like this:

    33 carlos 4455

## Menu

| Option | What it does |
|---|---|
| 1 | Add a record to the source file and insert its id in the tree |
| 2 | Search for an id and print the record |
| 3 | Save the tree nodes to the index file |
| 4 | Free the tree and exit |
