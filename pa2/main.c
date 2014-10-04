/*
 * sorted-list.c
 */

#include	<string.h>
#include	"sorted-list.h"
#include	<stdio.h>

int compareInts(void *p1, void *p2)
{
	int i1 = *(int*)p1;
	int i2 = *(int*)p2;

	return i1 - i2;
}

int compareDoubles(void *p1, void *p2)
{
	double d1 = *(double*)p1;
	double d2 = *(double*)p2;

	return (d1 < d2) ? -1 : ((d1 > d2) ? 1 : 0);
}

int compareStrings(void *p1, void *p2)
{
	char *s1 = p1;
	char *s2 = p2;

	return strcmp(s1, s2);
}

//Destructor functions
void destroyBasicTypeAlloc(void *p){
	//For pointers to basic data types (int*,char*,double*,...)
	//Use for allocated memory (malloc,calloc,etc.)
	free(p);
}

void destroyBasicTypeNoAlloc(void *p) {
	//For pointers to basic data types (int*,char*,double*,...)
	//Use for memory that has not been allocated (e.g., "int x = 5;SLInsert(mylist,&x);SLRemove(mylist,&x);")
	return;
}


int main(int argc, char** argv)
{
	SortedListPtr list = SLCreate(compareInts, destroyBasicTypeNoAlloc);
	Node * ptr = malloc(sizeof(struct Node_));

	int a = 1, b = 2, c = 3, d = 4;
	
	printf("Inserting elements %d\t %d\t %d\t %d\t\n", a, b, c, d);
	SLInsert(list, &a);
	SLInsert(list, &b);
	SLInsert(list, &c);
	SLInsert(list, &d);
	
	/* prints all elements in list */
	for (ptr = list->head; ptr != NULL; ptr = ptr->next) {
		printf("%d\t", *(int*)ptr->data);
		}

	SortedListIteratorPtr iter = SLCreateIterator(list);
	int endoflist = 1;
	while (endoflist) {
		endoflist = (int)SLGetItem(iter);
		SLNextItem(iter);
	}
}
