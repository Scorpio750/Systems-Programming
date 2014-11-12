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
	puts(">> Entering createFileNode");
	if (pathname == NULL) {
		return NULL;
	}
	FileNode *newnode = (FileNode*)calloc(1, sizeof(FileNode));
	puts("Created new FileNode");
	newnode->pathname = (char*)calloc((strlen(pathname)+1),sizeof(char));
	strcpy(newnode->pathname, pathname);
	newnode->next = NULL;
	if (newnode != NULL) {
		printf("PathName of new FileNode [%s]\n",newnode->pathname);
		return newnode;
	}
	if (newnode->next->pathname == NULL){
		puts("For some reason, the Pathname of new FileNode is NULL");
		return NULL;
	}
	else {
		puts("For some reason the new FileNode is NULL");
		return NULL;
	}
}

LinkedList *createLL(char *pathname){
	puts(">> ENTERING createLL");
	LinkedList *newhead = (LinkedList*)malloc(sizeof(LinkedList));
	puts("Created New LinkedList Struct");
	newhead->head = createFileNode(pathname);
	puts("Create new  FileNode head for new LinkedList");
	printf("This should be the pathname of the newhead of LL [%s]\n",newhead->head->pathname);
	return newhead;
}

//Destroy Functions
void destroyList(FileNode *head){
	puts(">> ENTERING destroyList");
	if(head == NULL){
		puts("The FileNode head that is to be destroyed is NULL");
		return;
	}
	destroyList(head->next);
	if (head->pathname != NULL){
		puts("FREEING THE PATHNAME");
		printf("This is the pathnamed that is going to be freed [%s]\n", head->pathname);
		free(head->pathname);
	}
	if (head != NULL){
		puts("FREEING THE HEAD");
		free(head);
	}
	puts("END OF DESTROYLIST");
	return;
}

void destroyNode(TNode *node){
	puts(">> ENTERING destroyNode");
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
	puts(">> ENTERING destroyFileNode");
	if (node->pathname != NULL){
		free(node->pathname);
	}
	if (node != NULL){
		free(node);
	}
	return;
}

int hash(char c){
	char * index = strchr(acceptable, c);
	if (index == NULL) {
		return -1;
	}
	return (int)(index - acceptable);
}

TNode *addNode(char *buffer, TNode *root){
	puts(">> ENTERING addNode");
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
		//printf("letter at ptr [%c]\n", ptr->c);
	}
	ptr->isWord = true;
	return ptr;
}

FileNode *addList(FileNode *node, char *buffer){
	puts(">> ENTERING addList");
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
			//printf("STATE: [%d] WORD [%s]\n", state, buffer);
			ptr = addNode(buffer,root);
			//printf("ADDED NODE\n");
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
}


void printLinkedList(LinkedList *LL) {
	puts(">> ENTERING printLinkedList");
	FileNode *ptr;
	if(LL == NULL) {
		puts("Search terms cannot be found");
		return;
	}	
	if(LL->head == NULL) {
		puts("LL is not NULL, BUT the head is...not good.");
		return;
	}
	for (ptr = LL->head; ptr != NULL; ptr = ptr->next){
		printf("%s\n",ptr->pathname);
	}
	return;
}

// Removes FileNode curr
LinkedList *removeNode(FileNode *prev, FileNode *curr, LinkedList *LL) {
	puts(">> ENTERING removeNode");
	if (prev == NULL && curr == NULL){
		puts("The prev node and the node that should be removed is NULL");
		return NULL;
	}
	if (prev == NULL) {
		puts("The Node that should be removed is the head of the LL");
		prev = curr;
		curr = curr->next;
		LL->head = curr;
		if (prev != NULL){
			puts("The node that we are removing is not NULL");
			destroyFileNode(prev);
		}
	}
	else {
		puts("The node that should be removed is not the head of the Linked List");
		prev->next = curr->next;
	}
	return LL;
}

LinkedList *insertFile(LinkedList *LL, FileNode *node, int sa){
	puts(">> ENTERING insertFile");
	FileNode *ptr = NULL;
	FileNode *ptr2 = NULL;
	FileNode *prev2 = NULL;
	LinkedList *tmp = NULL;
	FileNode *tptr = NULL;
	FileNode *newnode = NULL;

	// creates new result LL if none exists
	if (LL == NULL){
		puts("The Linked List is NULL: Create a New Linked List");
		for (ptr = node; ptr != NULL; ptr = ptr->next){
			printf("This is the pathname of the FileNode in the TNode: [%s]\n", ptr->pathname);
			if (LL == NULL){
				puts("LL struct IS NULL: Create a NEW LinkedList struct && head");
				LL = createLL(ptr->pathname);
				ptr2 = LL->head;
				printf("This is PTR2(LL->head)->pathname [%s]\n", ptr2->pathname);
			}else{
			//	puts("LL struct is no longer NULL: add to front");
				puts("Time to create a newFileNode");
				/*	
			  newnode = createFileNode(ptr->pathname);
				newnode->next = ptr2;
				LL->head = newnode;
				printf("This is the newly reassigned LL head pathname [%s]\n", LL->head->pathname);
				*/
				
				puts("LL struct is no longer NULL: add to end");
				newnode = createFileNode(ptr->pathname);
				ptr2->next = newnode;
				ptr2 = ptr2->next;	
				printf("THE PATH NAME THAT SHOULD BE ADDED [%s]\n",ptr2->pathname);
			
			}
		}
		return LL;
	}

	printf("MADE the TEMPORARY LINKEDLIST\n");
    printf("STATE : %d\n", sa);
    // state is so
	//printf("LL IS NOT NULL: This is the LL->head->pathname [%s]\n", LL->head->pathname);
	if (sa == 0){
		// compares the filenodes in your indexed list to avoid duplicates
		puts("THE FLAG IS [SO]");
		for (ptr = node; ptr != NULL; ptr = ptr->next){
			printf("This is the pathname of the FileNode in the TNode [%s]\n", ptr->pathname);
			for (ptr2 = LL->head; ptr2 != NULL; ptr2 = ptr2->next){
				printf("This if the pathname of the FileNode in the LL [%s]\n", ptr2->pathname);	
				if (strcmp(ptr->pathname, ptr2->pathname) == 0) {
					printf("The TNode FileNode [%s] and the Ll FileNode [%s] are EQUAL\n", ptr->pathname, ptr2->pathname);
					break;
				}
				//prev2 = ptr2;
				if (ptr2->next == NULL){
					puts("We've reached the end of the list, add a newnode to the end");
					newnode = createFileNode(ptr->pathname);
					ptr2->next = newnode;
					//prev2->next = newnode;
				}
			}
		}
	}

	// state is sa
	if (sa == 1){
		// making temporary copy of LL
		puts("THE FLAG IS [SA]");
		puts("CREATING A TEMPORARY LINKED LIST TO TRAVERSE THROUGH");
		for (ptr2 = LL->head; ptr2 != NULL; ptr2 = ptr2->next){
			printf("This is the pathname of the FileNode in the LL [%s]\n", ptr2->pathname);
			if (tmp == NULL){
				puts("tmp LL struct is NULL");
				tmp = createLL(ptr2->pathname);
				tptr = tmp->head;
				printf("This is the new head for for the tmp LL Struct  [%s]\n", tptr->pathname);
				continue;
			}else{
				newnode = createFileNode(ptr2->pathname);
				newnode->next = tptr;
				tmp->head = newnode;
			}
		}

		puts("Made the temporary LinkedList");

		prev2 = NULL;
		if (LL != NULL){
			ptr2 = LL->head;
		}

		for (tptr = ptr2; tptr != NULL; tptr = tptr->next){
			//printf("This is the FileNode pathname in the tmp LL [%s]\n", ptr2->pathname);
			for (ptr = node; ptr != NULL; ptr = ptr->next){
				//printf("This is the FileNode pathname in the TNode [%s]\n", ptr->pathname);
				if (strcmp(tptr->pathname, ptr->pathname) == 0){
					puts("THE LL FileNode pathname && The FileNode pathname of TNode is EQUIVALENT: Should go into the next interation of the tmp LL");
					//printf("The LL FileNode pathname [%s] = The FileNode pathname of TNode [%s]\n", tptr->pathname, ptr->pathname);
					break;		
				}
				if (ptr->next == NULL){
					//printf("We've reached the end of the TNode FileNode linkedlist:: Remove the node from LL\n");
					LL = removeNode(prev2, ptr2, LL);
					if (LL->head == NULL){
						return NULL;
					}
				}
			}
			prev2 = ptr2;
			ptr2 = ptr2->next;
		}

		if (tmp->head != NULL){
			puts("DESTROY THE TEMPORARY LINKED LIST");
			destroyList(tmp->head);
		}
		if (tmp != NULL){
			puts("FREE THE TEMP LL STRUCT");
			free(tmp);
		}
	}
	puts("Exited states");
	return LL;
}

LinkedList *printFiles(LinkedList *LL, char *filename, TNode *root, int flag) {
	puts("Inside printFiles");
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
		c = filename[i];
		index = hash(c);
		if (ptr->children[index] == NULL) {
			if (flag == 0){
				return LL;
			}
			return NULL;
		}
		if (ptr->children[index]->isWord && i == strlen(filename)-1) {
			printf("WE HAVE REACHED THE WORD, this is the chara [%c]\n", ptr->children[index]->c);
			LL = insertFile(LL, ptr->children[index]->head, flag);
			return LL;
		}
		ptr = ptr->children[index];
		printf("THIS IS THE CHARCTER AS WE TRAVERSE THROUGH [%c]\n", ptr->c);
		
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
		if (list != NULL && list->head != NULL){
			destroyList(list->head);
			puts("RESET list equal to NULL");
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
				puts("Out of printFiles");
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
				puts("Invalid Input.");
				exit(1);
			}
		}
	}

	if (tree != NULL && tree->root != NULL){
		puts("DESTROY TREE");
		destroyNode(tree->root);
	}
	if(list != NULL && list->head != NULL){
		puts("DESTROY LIST");
		destroyList(list->head);
	}
	if(tree != NULL){
		puts("DESTROY TREE STRUCT");
		free(tree);
	}
	if(list != NULL){
		puts("DESTROY LL STRUCT");
		free(list);
	}

	free(query_answer);
	return 0;
}

