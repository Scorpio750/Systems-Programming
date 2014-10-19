#ifndef INDEXER_H
#define INDEXER_H
#define acceptable "abcdefghijklmnopqrstuvwxyz0123456789"

#include <stdio.h>

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

void insertHashTable(Hash_Table * hash, char * word, char * filename);
Hash_Table * createTable();
void dump_to_file(Hash_Table, char * filename);

int checkDir(char * file);
int checkFile(char * file);

#endif
