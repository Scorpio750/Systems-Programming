#include <string.h>
#include "sorted-list.h"

SortedListPtr SLCreate(CompareFuncT cf, DestructFuncT df) {
	SortedListPtr SL = malloc(sizeof(struct SortedList));
	SL->comparator = cf;
	SL->destructo_disc = df;
	return SL;
}

void SLDestroy(SortedListPtr list) {
	Node *ptr = malloc(sizeof(Node));
	Node *prev = malloc(sizeof(Node));;
	for (ptr = list->head; ptr != NULL; ptr = ptr->next) {
		list->destructo_disc(ptr->data);
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
		Node* ptr = malloc(sizeof(struct Node_));
		Node* newNode = malloc(sizeof(Node));
		int cmpresult;

		newNode->data = newObj;

		/* for each node, compares it and increments it based on the return value */
		for (ptr = list->head; ptr != NULL; ptr = ptr->next) {
			cmpresult = list->comparator(newObj, ptr->data);
			if (cmpresult == -1) {
				continue;
			}
			else {
				newNode->next = ptr->next;	
				ptr->next = newNode;
				return 1;
			}
		}
		return 0;
	}
}

int SLRemove(SortedListPtr list, void *newObj) {

}

SortedListIteratorPtr SLCreateIterator(SortedListPtr list) {
	Node iter = malloc(sizeof(Node));

}

void SLDestroyIterator(SortedListIteratorPtr iter) {

}

void * SLGetItem(SortedListIteratorPtr iter) {

}

void * SLNextItem(SortedListIteratorPtr iter) {

}
