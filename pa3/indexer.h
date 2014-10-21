#ifndef INDEXER_H
#define INDEXER_H
#define acceptable "0123456789abcdefghijklmnopqrstuvwxy"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>
#include <ctype.h>
#include <sys/stat.h>

#define true 1
#define false 0
typedef unsigned char bool;

struct File_Node_ {
	char * filename;
	int occurrences;
	struct File_Node_ * next;
	struct File_Node_ * prev;
};

typedef struct File_Node_ File_Node;

struct Prefix_Node_ {
	char c;
	int depth;
	bool isWord;
	File_Node * head;
	struct Prefix_Node_ ** next;
};


typedef struct Prefix_Node_ Prefix_Node;

struct Hash_Table_ {
	Prefix_Node * head;
	Prefix_Node * ptr;
};

typedef struct Hash_Table_ Hash_Table;

/* Hash Table functions */
Hash_Table * createTable();
// void insertHashTable(Hash_Table * hash, char c, char * filename);
void insertTrie(FILE * file, Hash_Table *table, char * filename);
int hash(char c);
void insertFileNode(char * filename, Hash_Table inv_index);
void readFile(Hash_Table * hash, char * path);
Prefix_Node * createNode(char c);

/* File System functions */
void recurseDir(Hash_Table * hash, char * path);
int checkDir(char * file);
int checkFile(char * file);

/* Output functions */
bool isEmpty(Prefix_Node ** ptr);
void recursivePrintTree(char * buffer, Prefix_Node * ptr, FILE * file);
void dump_to_file(Hash_Table * inv_index, char * filename);
#endif
