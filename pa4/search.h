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

struct FileNode_{
	char *pathname;
	struct FileNode_ *next;
};

typedef struct FileNode_ FileNode;

struct TNode_{
	char c;
	bool isWord;
	struct TNode_ **children;
	FileNode *head;
};

typedef struct TNode_ TNode;

<<<<<<< HEAD
struct Root_ {
=======
struct Tree_{
>>>>>>> 360935799be19c15949e87bbadb91aa36b1b69ae
	TNode *root;
};

<<<<<<< HEAD
TNode *createNode(char c);
void addNode(FILE *file, TNode *root);
void tokenize(query_answer);
=======
typedef struct Tree_ Tree;

//Functions
TNode *createNode(char c);
Tree *createRoot();
FileNode *createFileNode(char *pathname);
int hash(char c);
TNode *addNode(char *buffer, TNode *root);
FileNode *addList(FileNode *node, char *buffer);
void readIndex(FILE *file, TNode *root);
>>>>>>> 360935799be19c15949e87bbadb91aa36b1b69ae

#endif
