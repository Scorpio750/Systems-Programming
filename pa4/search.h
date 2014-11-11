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

struct LinkedList_{
	FileNode *head;
};

typedef struct LinkedList_ LinkedList;

struct TNode_{
	char c;
	bool isWord;
	struct TNode_ **children;
	FileNode *head;
};
typedef struct TNode_ TNode;

struct Tree_{
	TNode *root;
};
typedef struct Tree_ Tree;

//Functions
TNode *createNode(char c);
Tree *createRoot();
FileNode *createFileNode(char *pathname);
LinkedList *createLL(char *pathname);
void destroyList(FileNode *head);
void destroyNode(TNode *node);
void destroyFileNode(FileNode *node);
int hash(char c);
TNode *addNode(char *buffer, TNode *root);
FileNode *addList(FileNode *node, char *buffer);
void readIndex(FILE *file, TNode *root);
void printLinkedList(LinkedList *LL);
void removeNode(FileNode *prev, FileNode *curr, LinkedList *LL);
LinkedList *insertFile(LinkedList *LL, FileNode *node, int sa);
void printFiles(LinkedList *LL, char *filename, TNode *root, int sa);
#endif
