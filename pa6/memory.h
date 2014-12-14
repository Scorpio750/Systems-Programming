#ifndef MEMORY_H
#define MEMORY_H

/*
 * Alison Wong
 * Patrick Wu
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#define malloc( x ) mymalloc( x, __FILE__, __LINE__)
#define calloc( x ) mycalloc( x ,__FILE__,__LINE__)
#define realloc( x ) myrealloc( x ,__FILE__,__LINE__)
#define free( x ) myfree( x, __FILE__, __LINE__)
#define MESIZE sizeof(struct MemEntry)
#define BLOCKSIZE (1024 * 1024)
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

 char Big_Block[BLOCKSIZE];

struct MemEntry{
	unsigned int FLAG;
	struct MemEntry *prev;
	struct MemEntry *succ;
	int isFree;
	int size;
};

void leak_detection();
void myfree (void *p, char *file, unsigned int line);
void *mymalloc (unsigned int size, char *file, unsigned int line);
void *mycalloc (unsigned int size, char *file, unsigned int line);

#endif
