/*
 * Alison Wong
 * Patrick Wu
 */

#ifndef INDEXER_H
#define INDEXER_H
#define acceptable "0123456789abcdefghijklmnopqrstuvwxyz"

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
	char * pathname;
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
};

typedef struct Hash_Table_ Hash_Table;

/* Hash Table functions */

/*
 * createTable() allocates memory for a Hash_Table which holds the root of the
 * prefix Tree.
 */
Hash_Table *createTable();

/*
 * createNode() allocates memory for a Prefix_Node and for the Prefix_Node 
 * array that is stored in each Prefix_Node.
 */
Prefix_Node *createNode(char c);

/*
 * createFileNode() allocated memory for a File_Node and the pathname within
 * the File_Node.
 */
File_Node *createFileNode(char *pathname);

/*
 * swap() sorts the linked list.
 */
void swap(Prefix_Node *node, File_Node *small, File_Node *big);

/*
 * checkList() will create a new Linked List if it doesn't already exist. It
 * also handles the updates and will add a new node at the end of the list if
 * the file doesn't already exist.
 */
void checkList(Prefix_Node *node, char *pathname);

/*
 * hash() will return the index of the array.
 */
int hash(char c);

/*
 * inserTrie() will create the prefix tree given a file.
 */
void insertTrie(FILE * file, Hash_Table *table, char * filename);

/* File System functions */

/*
 * checkDir() will return 1 if it is a dir, 0 otherwise.
 */
int checkDir(char * file);

/*
 * checkFile() will return 1 if it is a file, 0 otherwise.
 */
int checkFile(char * file);

/*
 * recurseDir() recurses through the directories if there exists 
 * sub-directories.
 */
void recurseDir(Hash_Table * hash, char * path);

/* Output functions */

/*
 * isEmpty() checks if the array is empty.
 */
bool isEmpty(Prefix_Node ** ptr);

/*
 * formatOutput() will format the output and write to the new file. 
 */
char * formatOutput(char * buffer, File_Node * head, char *formatted_string);

/*
 * recursivePrintTree() recurses through the Tree and calls on formatOutput().
 */
void recursivePrintTree(char *buffer, Prefix_Node * ptr, FILE *file);

/*
 * printTree() will call on recurseivePrintTree() to write to file.
 */
void printTree(Prefix_Node * head, FILE * file);

/*
 * dump_to_file() calls on printTree().
 */
void dump_to_file(Hash_Table * inv_index, char * filename);

/* Destroy Functions */

/*
 * destroyList() recursively frees the linked list and the pathnames within 
 * the linked list.
 */
void destroyList(File_Node *head);

/*
 * destroyNode() recursively frees the all the prefix nodes in the tree.
 */
void destroyNode(Prefix_Node *node);

#endif
