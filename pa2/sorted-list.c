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
	Node ptr;
	Node prev;
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
