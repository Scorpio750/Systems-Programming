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
#define free( x ) myfree( x, __FILE__, __LINE__)
#define MESIZE sizeof(struct MemEntry)
#define BLOCKSIZE (1024 * 1024)

static char Big_Block[BLOCKSIZE];

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
