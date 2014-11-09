#ifndef SEARCH_H
#define INDEXER_H
#define acceptable "0123456789abcdefghijklmnopqrstuvwxyz"

/*
 * Alison Wong
 * Patrick Wu
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef enum {false, true} bool;

struct FileNode_ {
	char *pathname;
	struct FileNode_ *next;
};
typedef struct FileNode_ FileNode;

struct TNode_{
	char c;
	bool isWord;
	struct TNode **children;
	FileNode *head;
};
typedef struct TNode_ TNode;

struct Root_ {
	TNode *root;
};
typedef struct Root_ Root;

TNode *createNode(char c);
void addNode(FILE *file, TNode *root);
void tokenize(query_answer);

#endif
