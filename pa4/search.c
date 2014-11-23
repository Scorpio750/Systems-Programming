/*
 * Alison Wong
 * Patrick Wu
 */
#include "search.h"

/*
 
bool isEmpty(TNode **array) {
	int i;
	for (i=0; i <36; i++){
		if (array[i] != NULL)
			return false;
	}
	return true;
}

*/

//Create Functions
TNode *createNode(char c) {
	TNode *node = (TNode*)malloc(sizeof(TNode));
	node->c = c;
	node->children = (TNode**)calloc(36, sizeof(TNode*));
	node->depth = 0;
	node->isWord = false;
	node->head = NULL;
	return node;
}

Tree *createRoot() {
	Tree *tree = malloc(sizeof(Tree));
	tree->root = createNode(' ');
	return tree;
}

FileNode *createFileNode(char *pathname) {
	if (pathname == NULL) {
		return NULL;
	}
	FileNode *newnode = (FileNode*)calloc(1, sizeof(FileNode));
	newnode->pathname = (char*)calloc((strlen(pathname)+1),sizeof(char));
	strcpy(newnode->pathname, pathname);
	newnode->next = NULL;
	return newnode;
}

LinkedList *createLL(char *pathname) {
	LinkedList *newhead = (LinkedList*)malloc(sizeof(LinkedList));
	newhead->head = createFileNode(pathname);
	return newhead;
}

//Destroy Functions
void destroyList(FileNode *head) {
	if(head == NULL){
		return;
	}
	destroyList(head->next);
	if (head->pathname != NULL){
		if ((int)strlen(head->pathname) != 0){
			free(head->pathname);
			head->pathname = NULL;
		}
	}
	free(head);
	head = NULL;
	return;
}

void destroyNode(TNode *node) {
	int i;
	if (node == NULL)
		return;
	for (i = 0; i < 36; i++){
		if (node->children[i] == NULL)
			continue;
		destroyNode(node->children[i]);
	}
	destroyList(node->head);
	free(node->children);
	node->children = NULL;
	free(node);
	node = NULL;
}

// Data Structures functions
void destroyFileNode(FileNode *node) {
	if (node != NULL && node->pathname != NULL){
		free(node->pathname);
		node->pathname = NULL;
		free(node);
		node = NULL;
	}
	return;
}

int hash(char c) {
	char * index = strchr(acceptable, c);
	if (index == NULL) {
		return -1;
	}
	return (int)(index - acceptable);
}

TNode *addNode(char *buffer, TNode *root) {
	int index;
	TNode *ptr = root;
	int i;
	char c;

	for (i = 0; i < strlen(buffer); i++){
		c = buffer[i];
		index = hash(c);
		if (ptr->children[index] == NULL){
			ptr->children[index] = createNode(c);
			ptr->children[index]->depth = ptr->depth+1;
		}
		ptr = ptr->children[index];
	}
	ptr->isWord = true;
	return ptr;
}

FileNode *addList(FileNode *node, char *buffer) {
	FileNode *newnode = createFileNode(buffer);
	node->next = newnode;
	return newnode;
}

/*

void recursivePrint(char *buffer, TNode *node){
	if(isEmpty(node->children) == true)
		return;
	int i;
	int index;
	for(i=0;i<36;i++){
		if (node->children[i] == NULL)
			continue;
		index = node->children[i]->depth -1;
		buffer[index] = (char)node->children[i]->c;
		if (node->children[i]->isWord){
			buffer[index+1] = '\0';
		}
		recursivePrint(buffer, node->children[i]);
	}
	return;
}


void printTree(TNode *root) {
	char *buffer = (char *)calloc(105, sizeof(char));
	TNode *ptr = root;
	recursivePrint(buffer,ptr);
    free(buffer);
    return;
}

*/

// File I/O Functions
void readIndex(FILE *file, TNode *root) {
	int state = 0;
	char *buffer = (char *)malloc(sizeof(char) * 1024);
	char i;
	TNode *ptr;
	FileNode *llptr;

	do{
		i = fscanf(file, "%s", buffer);
		if(i == -1){
			break;
		}
		if (strcmp(buffer, "</list>") == 0){
			state = 0;
		}
		else if (state == 0){
			state = 1;
		}
		else if (state == 1){
			ptr = addNode(buffer,root);
			state = 2;
		}
		else if (state == 2){
			if (ptr->head == NULL){
				llptr = createFileNode(buffer);
				ptr->head = llptr;
			}
			else{
				llptr = addList(llptr, buffer);
			}
		}
	}while(i != EOF);
	fclose(file);
  free(buffer);
  return;
}


void printLinkedList(LinkedList *LL) {
	FileNode *ptr;
	if (LL == NULL){
		puts("Search terms cannot be found");
		return;
	}else if (LL->head == NULL) {
		puts("Search terms cannot be found");
		return;
	}	

	for (ptr = LL->head; ptr != NULL; ptr = ptr->next){
		if (strlen(ptr->pathname) == 0){
			continue;
		}
		printf("%s\n",ptr->pathname);
	}
	return;
}

// Removes FileNode curr
LinkedList *removeNode(FileNode *prev, FileNode *curr, LinkedList *LL) {
	if (prev == NULL && curr == NULL){
		LL->head = NULL;
		return LL;
	}
	if (prev == NULL) {
		prev = curr;
		curr = curr->next;
		LL->head = curr;
		destroyFileNode(prev);
	}
	else {
		prev->next = curr->next;
		destroyFileNode(curr);
	}
return LL;
}

LinkedList *insertFile(LinkedList *LL, FileNode *node, int sa){
	FileNode *ptr = NULL;
	FileNode *ptr2 = NULL;
	FileNode *prev2 = NULL;
	LinkedList *tmp = NULL;
	FileNode *tptr = NULL;
	FileNode *newnode = NULL;
	FileNode *ptr3 = NULL;

	// creates new result LL if none exists
	if (LL == NULL){
		LL = createLL(node->pathname);
		ptr2 = LL->head;
		for (ptr = node->next; ptr != NULL; ptr = ptr->next){
			newnode = createFileNode(ptr->pathname);
			ptr2->next = newnode;
			ptr2 = ptr2->next;	
		}
		return LL;
	}

  // state is so
	if (sa == 0){
		// compares the filenodes in your indexed list to avoid duplicates
		for (ptr = node; ptr != NULL; ptr = ptr->next){
			for (ptr2 = LL->head; ptr2 != NULL; ptr2 = ptr2->next){
				if (strcmp(ptr->pathname, ptr2->pathname) == 0) {
					break;
				}
				if (ptr2->next == NULL){
					newnode = createFileNode(ptr->pathname);
					ptr2->next = newnode;
				}
			}
		}
		return LL;
	}

	// state is sa
	if (sa == 1){
		if (LL->head == NULL){
			return LL;
		}
		//making a temporary linked list
		for (ptr2 = LL->head; ptr2 != NULL; ptr2 = ptr2->next){
			if (tmp == NULL){
				tmp = createLL(ptr2->pathname);
				tptr = tmp->head;
				continue;
			}else{
				newnode = createFileNode(ptr2->pathname);
				tptr->next = newnode;
				tptr = newnode;
			}
		}

		prev2 = NULL;
		ptr2 = LL->head;
		ptr3 = ptr2->next;

		for (tptr = tmp->head; tptr != NULL; tptr = tptr->next){
			for (ptr = node; ptr != NULL; ptr = ptr->next){
				if (strcmp(tptr->pathname, ptr->pathname) == 0){
					prev2 = ptr2;
					ptr2 = ptr3;
					if (ptr3 != NULL){
						ptr3 = ptr3->next;
					}
					break;		
				}
				if (ptr->next == NULL){
					LL = removeNode(prev2, ptr2, LL);
					ptr2 = ptr3;
					if (ptr3 != NULL){
						ptr3 = ptr3->next;
					}
					if (LL->head == NULL){
						destroyList(tmp->head);
						free(tmp);
						return LL;
					}
				}
			}
		}
		destroyList(tmp->head);
		free(tmp);
	}
	return LL;
}

LinkedList *printFiles(LinkedList *LL, char *filename, TNode *root, int flag) {
	TNode *ptr = root;
	if (ptr == NULL){
		fprintf(stderr, "Indexer does not exist\n");
		return NULL;
	}

    int i;
    int index;
    char c;

	// goes through each char of the filename, hashes it, and compares it to the tree
	for (i = 0; i < strlen(filename); i++){
		c = tolower(filename[i]);
		index = hash(c);
		if (ptr->children[index] == NULL) {
			if (flag == 0){
				return LL;
			}else {
				if (LL != NULL && LL->head != NULL) {
					destroyList(LL->head);
					LL->head = NULL;
				}
				return LL;
			}
		}
		if (ptr->children[index]->isWord && i == strlen(filename)-1) {
			LL = insertFile(LL, ptr->children[index]->head, flag);
			return LL;
		}
		ptr = ptr->children[index];
		
	}
	return LL;
}

int main (int argc, char **argv) {
    // LinkedList *searchterms = NULL;

    if (argc != 2) {
        fprintf(stderr, "Invalid number of arguments.\n");
        return 1;
    }

    FILE *index = fopen(argv[1], "r");
    if (index == NULL){
        fprintf(stderr, "File does not exist.\n");
        return 1;
    }
    int nbytes = 256;
    char * query_answer = malloc(nbytes * sizeof(char) + 1);
    char * token;

    Tree *tree = createRoot();
    LinkedList *list = NULL;

    readIndex(index,tree->root);

    // Query Menu
    for (;;) {
        puts("Enter your query:");
        if (list != NULL) {
          if (list->head != NULL) {
						destroyList(list->head);
					}
					free (list);
					list = NULL;
        }

        // if (query_answer) free(query_answer);
        int n = getline(&query_answer, (size_t *)&nbytes, stdin);
        if (n == -1) {
            fprintf(stderr, "Error: unable to read from input stream");
            goto end;
        }
        query_answer[n-1] = '\0';
        if (!strcmp(query_answer,"q")) {
            puts("Exiting program");
            goto end;
        }
        else {
            char * flag = strsep(&query_answer, " ");

            //so
            if (!strcmp("so", flag)) {
                for (token = strsep(&query_answer, " ");
                        token; 
                        token = strsep(&query_answer, " ")) {
                    list = printFiles(list, token, tree->root, 0);
                }	
                printLinkedList(list);
            }

            //sa
            else if (!strcmp("sa", flag)) {
                list = NULL;
                for (token = strsep(&query_answer, " ");
                        token;
                        token = strsep(&query_answer, " ")) {

                    list = printFiles(list, token, tree->root, 1);
										if (list == NULL){
											break;
										}
                    if(list->head == NULL){
											break;
                    }
                }
                printLinkedList(list);
            }
            // only one word to be searched
            else {
                puts("Invalid Input.");
            }
        }
    }
// frees all the things
end:
    if (tree && tree->root){
        destroyNode(tree->root);
    }
    if(tree){
        free(tree);
    }
    if (list) {
        free(list);    
    }
    if (query_answer) { 
//        free(query_answer);
        }
    return 0;
}
