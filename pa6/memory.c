/*
 * Alison Wong
 * Patrick Wu
 */

#include "memory.h"

void leak_detection() {
	struct MemEntry *ptr = (struct MemEntry *)(&Big_Block);
	unsigned int size = 0;
	unsigned int blocks = 0;

	while (ptr != NULL){
		if (ptr->isFree == 0) {
			size += ptr->size;
			blocks++;
		}
		ptr = ptr->succ;
	}
	if (size != 0){
		fprintf(stderr, ANSI_COLOR_RED "[Leak Detection] [Bytes: %d Blocks: %d]\n", size, blocks);
		fprintf(stderr, ANSI_COLOR_RESET "");
	}
}



void myfree (void *p, char *file, unsigned int line) {
	struct MemEntry *ptr;
	struct MemEntry *pred;
	struct MemEntry *succ;
	void *start = &Big_Block;
	void *end = &Big_Block + 1;
	
	if (p < start || p >= end) {
		fprintf(stderr, ANSI_COLOR_RED "[ERROR] [Line: %d File: %s] Freeing a pointer that was never allocated\n", line, file);
		fprintf(stderr, ANSI_COLOR_RESET "");
		return;
	}

	ptr = (struct MemEntry *)((char*)p - MESIZE);

	if (ptr->FLAG == INT_MAX){
		if (ptr->isFree == 1){
			fprintf(stderr, ANSI_COLOR_RED "[ERROR] [Line: %d File: %s] Redundant free()ing of the same pointer\n", line, file);
			fprintf(stderr, ANSI_COLOR_RESET "");
			return;
		}
	}else{
		fprintf(stderr, ANSI_COLOR_RED "[ERROR] [Line: %d File: %s] Freeing pointers to dynamic memory that were not reutrned from malloc()\n", line, file);
		fprintf(stderr, ANSI_COLOR_RESET "");
		return;
	}


	if ((pred = ptr->prev) != 0 && pred->isFree){
		pred->size += MESIZE + ptr->size;
		pred->succ = ptr->succ;
		if (ptr->succ != 0){
			ptr->succ->prev = pred;
		}
	}else {
		ptr->isFree = 1;
		pred = ptr;
	}
	if ((succ = ptr->succ) != 0 && succ->isFree){
		pred->size += MESIZE + succ->size;
		pred->succ = succ->succ;
		if (succ->succ != 0){
			succ->succ->prev = pred;
		}
	}
}

void *mymalloc (unsigned int size, char *file, unsigned int line) {
	static int initialized = 0;
	static struct MemEntry *root = 0;
	static struct MemEntry *last = 0;
	struct MemEntry *p;
	struct MemEntry *succ;
	if (!initialized) {
		root = (struct MemEntry *) Big_Block;
		root->prev = root->succ = 0;
		root->size = BLOCKSIZE - MESIZE;
		root->isFree = 1;
		initialized = 1;
		root->FLAG = INT_MAX;
	}
	p  = root;
	while (p != 0) {
		if (p->size < size || !p->isFree) { //large enough? taken?
			p = p->succ;
		}else if (p->size < (size + MESIZE)) {
			p->isFree = 0;
			p->FLAG = INT_MAX;
			return (char*)p + MESIZE;
		}else {
			succ = (struct MemEntry *)((char*)p + MESIZE + size);
			succ->prev = p;
			succ->succ = p->succ;
			if (p->succ != 0) {
				p->succ->prev = succ;
			}
			p->succ = succ;
			succ->size = p->size - MESIZE - size;
			succ->isFree = 1;
			p->size = size;
			p->isFree = 0;
			last = (p == last) ? succ : last;
			p->FLAG = INT_MAX;
			return (char *)p + MESIZE;
		}
	}
	
	fprintf(stderr, ANSI_COLOR_RED "[ERROR] [Line: %d File: %s] Malloc Failed: Could not find space for allocation\n", line, file);
	fprintf(stderr, ANSI_COLOR_RESET "");
	return 0;
}

void *mycalloc (unsigned int size, char *file, unsigned int line) {
	void *ptr = mymalloc(size, file, line);
	memset(ptr, size, 0);  
	return ptr;
}

char *test_func() {
	char *test;
	test = malloc(50);
	strcpy(test, "Malloc Success");
	return test;
}

