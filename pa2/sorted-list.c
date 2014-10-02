#include <string.h>
#include "sorted-list.h"

SortedListPtr SLCreate(CompareFuncT cf, DestructFuncT df) {
	SortedListPtr SL = malloc(sizeof(struct SortedList));
	SL->comparator = cf;
	SL->destruct = df;
	SL->head = NULL;
	return SL;
}

void SLDestroy(SortedListPtr list) {
	Node ptr = malloc(sizeof(struct Node));
	Node prev = malloc(sizeof(struct Node));;
	for (ptr = list->head; ptr != NULL; ptr = ptr->next) {
		list->destruct(ptr->data);
		if (ptr != list->head) {
			prev->next = NULL;
			free(prev);
		}
		prev = ptr;
	}
}

int SLInsert(SortedListPtr list, void *newObj) {
	if (list == NULL || newObj == NULL) {
		return 0;
	}
	else {
		Node ptr = malloc(sizeof(struct Node));
		Node newNode = malloc(sizeof(struct Node));
		newNode->data = newObj;

		int cmpresult;
		/* for each node, compares it and increments it based on the return value */
		for (ptr = list->head; ptr != NULL; ptr = list->next) {
			cmpresult = list->comparator(newObj, ptr->data);
			if (cmpresult == -1) {
				continue;
			}
			else {
				newNode->next = ptr->next;	
				ptr->next = newNode;
				break;
			}
		}
	}
}

int SLRemove(SortedListPtr list, void *newObj) {

}

SortedListIteratorPtr SLCreateIterator(SortedListPtr list) {

}

void SLDestroyIterator(SortedListIteratorPtr iter) {

}

void * SLGetItem(SortedListIteratorPtr iter) {

}

void * SLNextItem(SortedListIterPtr iter) {

}
