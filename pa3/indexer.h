#ifndef INDEXER_H
#define INDEXER_H
#define acceptable "abcdefghijklmnopqrstuvwxyz0123456789"

#include <stdio.h>

#define true 1
#define false 0
typedef unsigned char bool;

struct File_Node_ {
	char * filename;
	int occurrences;
	struct File_Node * next;
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

Hash_Table * createTable();
void insertHashTable(Hash_Table * hash, char c, char * filename);
int hash(char c);
void insertFileNode(char * filename, Hash_Table inv_index);
void readFile(Hash_Table hash, char * path);
void recurseDir(Hash_Table hash, char * path);
int checkDir(char * file);
int checkFile(char * file);
void dump_to_file(Hash_Table * inv_index, char * filename);
#endif
