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
			if (cmpresult == 0) {
				return 0;
			}
			else if (cmpresult == -1) {
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
	if (list == NULL || newObj == NULL) {
		return 0;
	}

	Node ptr = malloc(sizeof(struct Node_));
	Node prev = malloc(sizeof(struct Node_));

	/* walks through list and compare node data to object*/
	for (ptr = list->head; ptr != NULL; ptr = ptr->next) {
		/* finds match */
		if (list->comparator(newObj, ptr->data) == 0) {
			/* unlinks target node */
			if (ptr != list->head) {
				prev->next = ptr->next;
				return 1;
			}
			else {
				list->head = ptr->next;
				return 1;
			}
		}
		prev = ptr;
	}
	return 0;
}

SortedListIteratorPtr SLCreateIterator(SortedListPtr list) {
	SortedListIteratorPtr iter = malloc(sizeof(struct SortedListIterator));
	iter->iter = list->head;
	return iter;
}

void SLDestroyIterator(SortedListIteratorPtr iter) {

}

void * SLGetItem(SortedListIteratorPtr iter) {

}

void * SLNextItem(SortedListIteratorPtr iter) {

}
