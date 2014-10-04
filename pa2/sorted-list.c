#include <string.h>
#include <stdio.h>
#include "sorted-list.h"

SortedListPtr SLCreate(CompareFuncT cf, DestructFuncT df) {
    SortedListPtr SL = malloc(sizeof(struct SortedList));
    SL->comparator = cf;
    SL->destructo_disc = df;
    SL->head = NULL;
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
        /* frees final node */
        if (ptr->next == NULL) {
            free(ptr);
            return;
        }
        prev = ptr;
    }
}

int SLInsert(SortedListPtr list, void *newObj) {
    if (list == NULL || newObj == NULL) {
        return 0;
    }
    else {
        Node* ptr;
        Node* prev;
        Node* newNode = malloc(sizeof(Node));
        int cmpresult;

        newNode->data = newObj;
        newNode->next = NULL;
        newNode->refcount = 0;

        /* if list is empty */
        if (list->head == NULL) {
            list->head = newNode;
            newNode->refcount++;
            return 1;
        }
        else {
            /* for each node, compares it and increments it based on the return value */
            for (ptr = list->head; ptr != NULL; ptr = ptr->next) {
                cmpresult = list->comparator(newObj, ptr->data);
                if (cmpresult == 0) {
                    printf("SAME VALUE ERROR\n");
                    return 0;
                }
                else if (cmpresult < 0) {
                    // End of linked list
                    if (ptr->next == NULL) {
                        ptr->next = newNode;
                        newNode->refcount++;
                        return 1;
                    }
                    else {
                        prev = ptr;
                        continue;
                    }
                }
                else {
                    if (ptr == list->head) {
                        newNode->next = ptr;
                        ptr->refcount++;
                        list->head = newNode;
                        return 1;

                    }
                    else {
                        newNode->next = ptr;	
                        ptr->refcount++;
                        prev->next = newNode;
                        newNode->refcount++;
                    }
                    return 1;
                }
                prev = ptr;
            }
        }
        return 0;
    }
}

int SLRemove(SortedListPtr list, void *newObj) {
    Node* ptr;
    Node* prev;

    if (list == NULL || newObj == NULL) {
        return 0;
    }

    /* walks through list and compare node data to object*/
    for (ptr = list->head; ptr != NULL; ptr = ptr->next) {
        /* finds match */
        if (list->comparator(newObj, ptr->data) == 0) {
            /* unlinks target node */
            if (ptr != list->head) {
                prev->next = ptr->next;
                ptr->refcount--;
                return 1;
            }
            else {
                ptr->refcount--;
                list->head = ptr->next;
                return 1;
            }
        }
        prev = ptr;
    }
    return 0;
}

SortedListIteratorPtr SLCreateIterator(SortedListPtr list) {
    if(list == NULL || list->head == NULL){
        return NULL;
    }
    SortedListIteratorPtr iter = malloc(sizeof(struct SortedListIterator));
    iter->iter = list->head;
    iter->iter->refcount++;
    return iter;
}

void SLDestroyIterator(SortedListIteratorPtr iter) {
    free(iter);
    return;
}

void * SLGetItem(SortedListIteratorPtr iter) {
    if (iter->iter == NULL) {
        return 0;
    }
    else {
        return iter->iter->data;
    }
}

void * SLNextItem(SortedListIteratorPtr iter) {
    Node* prev;

    if (iter->iter->next == NULL) {
        return NULL;
    }
    else {
        prev = iter->iter;
        iter->iter = iter->iter->next;
        iter->iter->refcount++;
        prev->refcount--;
        if (prev->refcount <= 0) {
            free(prev);
        }
        return iter->iter->data;
    }
}
