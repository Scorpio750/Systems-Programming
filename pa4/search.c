/*
 * Alison Wong
 * Patrick Wu
 */
#include "search.h"

//Create Functions
TNode *createNode(char c){
	TNode *node = (TNode*)malloc(sizeof(TNode));
	node->c = c;
	node->children = (TNode**)calloc(36, sizeof(TNode*));
	node->isWord = false;
	node->head = NULL;
	return node;
}

Tree *createRoot(){
	Tree *tree = malloc(sizeof(Tree));
	tree->root = createNode(' ');
	return tree;
}

FileNode *createFileNode(char *pathname){
	if (pathname == NULL) {
		return NULL;
	}
	FileNode *newnode = (FileNode*)calloc(1, sizeof(FileNode));
	newnode->pathname = (char*)malloc((strlen(pathname)+1)*sizeof(char));
	strcpy(newnode->pathname, pathname);
	newnode->next = NULL;
	if (newnode != NULL) {
		return newnode;
	}
	else {
		return NULL;
	}
}

LinkedList *createLL(char *pathname){
	LinkedList *newhead = (LinkedList*)malloc(sizeof(LinkedList));
	newhead->head = createFileNode(pathname);
	return newhead;
}

//Destroy Functions
void destroyList(FileNode *head){
	if(head == NULL)
		return;
	destroyList(head->next);
	free(head->pathname);
	free(head);
	return;
}

void destroyNode(TNode *node){
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
	free(node);
}

// Data Structures functions
void destroyFileNode(FileNode *node){
	free(node->pathname);
	free(node);
}

int hash(char c){
	char * index = strchr(acceptable, c);
	if (index == NULL) {
		return -1;
	}
	return (int)(index - acceptable);
}

TNode *addNode(char *buffer, TNode *root){
	int index;
	TNode *ptr = root;
	int i;
	char c;

	for (i = 0; i < strlen(buffer); i++){
		c = buffer[i];
		index = hash(c);
		if (ptr->children[index] == NULL){
			ptr->children[index] = createNode(c);
		}
		ptr = ptr->children[index];
	}
	ptr->isWord = true;
	return ptr;
}

FileNode *addList(FileNode *node, char *buffer){
	FileNode *newnode = createFileNode(buffer);
	node->next = newnode;
	return newnode;
}

// File I/O Functions
void readIndex(FILE *file, TNode *root){
	printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>DOES readIndex() RUN???\n");
	int state = 0;
	char *buffer = (char *)malloc(sizeof(char) * 1024);
	char i;
	TNode *ptr;
	FileNode *llptr;

	do{
		i = fscanf(file, "%s", buffer);
	  //printf("STATE: [%d] STRING: [%s]\n", state, buffer);
		if(i == -1){
			//printf(">>>>>>EYE is NEGATIVE ONE\n");
			break;
		}
		if (strcmp(buffer, "</list>") == 0){
			//printf("STATE: [2] ACTUAL STATE[%d] WORD: [%s] </list>\n", state, buffer);
			state = 0;
		}
		else if (state == 0){
			//printf("STATE: [%d] WORD [%s]\n", state, buffer);
			state = 1;
		}
		else if (state == 1){
			//printf("STATE: [%d] WORD [%s]\n", state, buffer);
			ptr = addNode(buffer,root);
			//printf("ADDED NODE\n");
			state = 2;
		}
		else if (state == 2){
			//printf("STATE: [%d] WORD [%s]\n", state, buffer);
			if (ptr->head == NULL){
				//printf("Created New File Node\n");
				llptr = createFileNode(buffer);
				ptr->head = llptr;
			}
			else{
				//printf("Added to Linked List\n");
				llptr = addList(llptr, buffer);
			}
		}
	}while(i != EOF);
	fclose(file);
}


void printLinkedList(LinkedList *LL){
	FileNode *ptr;
	FileNode *prev = NULL;
	
	if(LL == NULL)
		return;
	if(LL->head == NULL)
		return;

	for (ptr = LL->head; ptr != NULL; ptr = ptr->next){
		printf("%s\n",ptr->pathname);
		prev = ptr;
	}

	destroyList(LL->head);
	free(LL);
}

void removeNode(FileNode *prev, FileNode *curr) {
	if (prev == NULL) {
		prev = curr;
		curr = curr->next;
		destroyFileNode(prev);
	}
	else {
		prev->next = curr->next;
		destroyFileNode(curr);
	}
	return;
}

LinkedList *insertFile(LinkedList *LL, FileNode *node, int sa){
	FileNode *ptr;
	FileNode *ptr2;
	FileNode *prev2 = NULL;
	LinkedList *tmp;
	FileNode *tptr;

	for (ptr2 = LL->head; ptr2 != NULL; ptr2 = ptr2->next){
		if (tmp == NULL){
			tmp = createLL(ptr2->pathname);
			tptr = tmp->head;
			continue;
		}
		tptr->next = ptr2;
	}
	
	if (sa == 0){
		for (ptr = node; ptr != NULL; ptr = ptr->next){
			if (LL == NULL){
				LL = createLL(ptr->pathname);
				continue;
			}
			for (ptr2 = LL->head; ptr2 != NULL; ptr2 = ptr2->next){		
				if (strcmp((ptr->pathname), ptr2->pathname) == 0)
					break;
				prev2 = ptr2;
				if (ptr2->next == NULL)
					prev2->next = createFileNode(ptr->pathname);
			}
		}
	}

	prev2 = NULL;

	if (sa == 1){
		for (tptr = tmp->head; tptr != NULL; tptr = tptr->next){
			ptr2 = tptr;
			for (ptr = node; ptr != NULL; ptr = ptr->next){
				if (strcmp((tptr->pathname), ptr->pathname) == 0)
					break;		
				if (ptr->next == NULL)
					removeNode(prev2, ptr2);
			}
			prev2 = tptr;
		}
	}

	if (tmp->head != NULL)
		destroyList(tmp->head);
	if (tmp != NULL)
		free(tmp);

	return LL;
}

void printFiles(LinkedList *LL, char *filename, TNode *root, int sa){
	printf("DOES printFILES() RUN?\n");
	TNode *ptr = root;
	if (ptr == NULL){
		fprintf(stderr, "Indexer DNE\n");
		return;
	}
	
	int i;
	int index;
	char c;
	for (i = 0; i < strlen(filename); i++){
		c = filename[i];
		index = hash(c);

		if (ptr->children[index] == NULL) {
			return;
		}

		if (ptr->children[i]->isWord) {
				LL = insertFile(LL, ptr->children[i]->head, sa);
		}
	}

}

int main (int argc, char **argv){
	char * query_answer = malloc(256 * sizeof(char) + 1);
	char * token;
 	if (argc != 2){
 		fprintf(stderr, "Invalid number of arguments.\n");
 		return 1;
 	}

 	FILE *index = fopen(argv[1], "r");
 	if (index == NULL){
 		fprintf(stderr, "File does not exist.\n");
 		return 1;
 	}

 	Tree *tree = createRoot();
	LinkedList *list = NULL;

	readIndex(index,tree->root);

	for (;;) {
		puts("Enter your query:");
		scanf("%s", query_answer);
		if (!strcmp(query_answer,"q")) {
			puts("Exiting program");
			exit(1);
		}
		else {
			puts("Query is not 'q'");
			if (!strcmp("so", strtok(query_answer, " "))) {
				puts("Logical V");
				while ((token = strtok(NULL, " \n"))) {
					printFiles(list, token, tree->root, 0);
				}	
			}
			else if (!strcmp("sa", strtok(query_answer, " "))) {
				puts("Logical ^");
				while ((token = strtok(NULL, " \n"))) {
					printFiles(list, token, tree->root, 1);
				}
			}
			// only one word to be searched
			else {
				puts("BAD QUERY");
				exit(1);
			}
		}
	}

	printf("%c\n",tree->root->children[10]->c);
	printf("%c\n",tree->root->children[10]->c);
	
	destroyNode(tree->root);
	destroyList(list->head);
	free(tree);
	free(list);
 	return 0;

}
