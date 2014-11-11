/*
 * Alison Wong
 * Patrick Wu
 */
#include "search.h"

bool isEmpty(TNode **array){
	int i;
	for (i=0; i <36; i++){
		if (array[i] != NULL)
			return false;
	}
	return true;
}

//Create Functions
TNode *createNode(char c){
	TNode *node = (TNode*)malloc(sizeof(TNode));
	node->c = c;
	node->children = (TNode**)calloc(36, sizeof(TNode*));
	node->depth = 0;
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
		printf("NEW NODE [%s]\n",newnode->pathname);
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
	if (node->pathname != NULL){
		free(node->pathname);
	}
	if (node != NULL){
		free(node);
	}
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
			ptr->children[index]->depth = ptr->depth+1;
		}
		ptr = ptr->children[index];
		printf("letter at ptr [%c]\n", ptr->c);
	}
	ptr->isWord = true;
	return ptr;
}

FileNode *addList(FileNode *node, char *buffer){
	FileNode *newnode = createFileNode(buffer);
	node->next = newnode;
	return newnode;
}

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

void printTree(TNode *root){
	char *buffer = (char *)calloc(105, sizeof(char));
	TNode *ptr = root;
	recursivePrint(buffer,ptr);
}

// File I/O Functions
void readIndex(FILE *file, TNode *root){
	printf("DOES readIndex() RUN???\n");
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
			printf("STATE: [%d] WORD [%s]\n", state, buffer);
			ptr = addNode(buffer,root);
			printf("ADDED NODE\n");
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


void printLinkedList(LinkedList *LL) {
	// puts("In linked list");
	FileNode *ptr;
	if(LL == NULL) {
		puts("Search terms cannot be found");
		return;
	}	
	if(LL->head == NULL) {
		puts("LL has nothing in it you fucking twat");
		return;
	}
	for (ptr = LL->head; ptr != NULL; ptr = ptr->next){
		printf("%s\n",ptr->pathname);
	}
	return;
}

void removeNode(FileNode *prev, FileNode *curr, LinkedList *LL) {
	if (prev == NULL) {
		prev = curr;
		curr = curr->next;
		LL->head = prev;
		if (prev != NULL){
			destroyFileNode(prev);
		}
	}
	else {
		prev->next = curr->next;
		destroyFileNode(curr);
	}
	return;
}

LinkedList *insertFile(LinkedList *LL, FileNode *node, int sa){
	FileNode *ptr = NULL;
	FileNode *ptr2 = NULL;
	FileNode *prev2 = NULL;
	LinkedList *tmp = NULL;
	FileNode *tptr = NULL;
	FileNode *newnode = NULL;

	// creates new result LL if none exists
	if (LL == NULL){
		for (ptr = node; ptr != NULL; ptr = ptr->next){
			printf("NODE FILES: [%s]\n", ptr->pathname);
			if (LL == NULL){
				printf("LL IS NULL\n");
				LL = createLL(ptr->pathname);
				ptr2 = LL->head;
			}else{
				printf("LL IS NOT NULL\n");
				printf("%s\n",ptr->pathname);
				newnode = createFileNode(ptr->pathname);
				printf("newnode pathname [%s]\n", newnode->pathname);
				printf("PTR2 current pathname [%s]\n", ptr2->pathname);
				ptr2->next = newnode;
				ptr2 = ptr2->next;	
				printf("THE PATH NAME THAT SHOULD BE ADDED [%s]\n",ptr2->pathname);
			}
		}
		return LL;
	}

	printf("%s\n", LL->head->pathname);
	// state is so
	if (sa == 0){
		// compares the filenodes in your indexed list to avoid duplicates
		printf("THIS IS WHERE SO IS BEING RUN\n");
		for (ptr = node; ptr != NULL; ptr = ptr->next){
			printf("%s\n", ptr->pathname);
			for (ptr2 = LL->head; ptr2 != NULL; ptr2 = ptr2->next){		
				if (strcmp(ptr->pathname, ptr2->pathname) == 0) {
					printf("%s and %s are EQUALL bitches\n", ptr->pathname, ptr2->pathname);
					break;
				}
				prev2 = ptr2;
				if (ptr2->next == NULL)
					prev2->next = createFileNode(ptr->pathname);
			}
		}

	}
	// state is sa
	if (sa == 1){
		// making temporary copy of LL
		printf("THIS IS WHERE SA IS BEING RUN\n");
		for (ptr2 = LL->head; ptr2 != NULL; ptr2 = ptr2->next){
			printf("LL NODES [%s]\n", ptr2->pathname);
			if (tmp == NULL){
				tmp = createLL(ptr2->pathname);
				tptr = tmp->head;
				printf("TMP NODE CREATED [%s]\n", tptr->pathname);
				continue;
			}
			tptr->next = createFileNode(ptr2->pathname);
		}
		printf("MADE the TEMPORARY LINKEDLIST\n");

		prev2 = NULL;
		for (tptr = tmp->head; tptr != NULL; tptr = tptr->next){
			ptr2 = tptr;
			for (ptr = node; ptr != NULL; ptr = ptr->next){
				if (strcmp((tptr->pathname), ptr->pathname) == 0){
					printf("LL PATHNAME [%s] NODE PATHNAME [%s]\n", tptr->pathname, ptr->pathname);
					break;		
				}
				if (ptr->next == NULL){
					printf("DOES THIS RUN\n");
					removeNode(prev2, ptr2, LL);
				}
				prev2 = ptr2;
			}
		}
		if (tmp->head != NULL){
			destroyList(tmp->head);
		}
		if (tmp != NULL){
			free(tmp);
		}
	}

	puts("Exited states");
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
	for (i = 0; i < strlen(filename); i++){
		c = filename[i];
		index = hash(c);
		if (ptr->children[index] == NULL) {
			if (flag == 0){
				return LL;
			}
			return NULL;
		}
		if (ptr->children[index]->isWord) {
			puts("ENTERING INSERTFILE");
			LL = insertFile(LL, ptr->children[index]->head, flag);
		}
		ptr = ptr->children[index];
	}
	return LL;
}

int main (int argc, char **argv) {
	int nbytes = 256;
	char * query_answer = malloc(nbytes * sizeof(char) + 1);
	char * token;
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

	Tree *tree = createRoot();
	LinkedList *list = NULL;

	readIndex(index,tree->root);

	printTree(tree->root);

	// Query Menu
	for (;;) {
		puts("Enter your query:");
		if (list != NULL){
			destroyList(list->head);
			list = NULL;
		}
		int n = getline(&query_answer, (size_t *)&nbytes, stdin);
		if (n == -1) {
			fprintf(stderr, "Error: unable to read from input stream");
			exit(1);
		}
		query_answer[n-1] = '\0';
		printf("QUERY ANSWER IS : %s\n", query_answer);
		if (!strcmp(query_answer,"q")) {
			puts("Exiting program");
			exit(1);
		}
		else {
			char * flag = strsep(&query_answer, " ");
			printf("FLAG: %s\n", flag);

			//so
			if (!strcmp("so", flag)) {
				for (token = strsep(&query_answer, " ");
						token; 
						token = strsep(&query_answer, " ")) {
					printf("QUERY ANSWER IS NOW : %s\n", query_answer);
					printf("TOKEN = %s\n", token);
					list = printFiles(list, token, tree->root, 0);
				}	
				printLinkedList(list);
			}
			
			//sa
			else if (!strcmp("sa", flag)) {
				for (token = strsep(&query_answer, " ");
						token;
						token = strsep(&query_answer, " ")) {
					printf("%s\n", token);
					list = printFiles(list, token, tree->root, 1);
				}
				printLinkedList(list);
			}
			// only one word to be searched
			else {
				printf("Query = %s\nToken = %s\n",query_answer, token);
				puts("BAD QUERY");
				exit(1);
			}
		}
	}

	if (tree != NULL && tree->root != NULL)
		destroyNode(tree->root);
	if(list != NULL && list->head != NULL)
		destroyList(list->head);
	if(tree != NULL)
		free(tree);
	if(list != NULL)
		free(list);

	free(query_answer);
	return 0;

}

