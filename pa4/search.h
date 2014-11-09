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

typedef enum {false, true} bool;

typedef struct FileNode{
	char *pathname;
	struct FileNode *next;
}FileNode;

typedef struct TNode{
	char c;
	bool isWord;
	struct TNode **children;
	FileNode *head;
}Tnode;

typedef struct Root{
	TNode *root;
}Root;

Root *createRoot();
Tnode *createNode(char c);
int hash(char c);
void addNode(FILE *file, TNode *root);
void readIndex(FILE *file, Root tree);


#endif