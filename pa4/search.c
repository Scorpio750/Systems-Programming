/*
 * Alison Wong
 * Patrick Wu
 */
#include "search.h"

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

void readIndex(FILE *file, TNode *root){
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

void SOprintFiles (LinkedList *LL, char *filename, TNode *root){
	TNode *ptr = root;
	FileNode *fptr;
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

		if (ptr->children[index] == NULL)
			return;

		if (ptr->children[i]->isWord){
			for (fptr = ptr->children[i]->head; fptr != NULL; fptr = fptr->next){
				LL = SOinsertFile(LL, fptr->pathname);
			}
		}
	}


}

LinkedList *SOinsertFile(LinkedList *LL, char *filename){
	if (LL == NULL){
		LL = createLL(filename);
		printf("%s \n", LL->head->pathname);
		return LL;
	}
	FileNode *ptr;
	FileNode *prev = NULL;
	for(ptr = LL->head; ptr != NULL;	ptr = ptr->next){
		if (strcmp((ptr->pathname),filename) == 0){
			break;
		}
		prev = ptr;
	}
	prev->next = createFileNode(filename);
	printf("%s \n", prev->next->pathname);
	return LL;
}

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
<<<<<<< HEAD
	LinkedList * list = NULL;
		
=======
	LinkedList *list = NULL;

>>>>>>> 7b35def5c55d9f624875db70c344cfe7c387838d
	while (1) {
		puts("Enter your query:");
		scanf("%s", query_answer);
		if (!strcmp(query_answer,"q")) {
			puts("Exiting program");
			exit(1);
		}
		else {
<<<<<<< HEAD
			puts("Query is not 'q'");
			if (!strcmp("so", strtok(query_answer, " "))) {
				puts("Logical V");
				while ((token = strtok(NULL, " "))) {
					SOprintFiles(list, token, tree->root);
				}	
=======
		//	tokenize(query_answer);
			
			if (!strncmp("so ", query_answer, 3)) {

>>>>>>> 7b35def5c55d9f624875db70c344cfe7c387838d
			}
			else if (!strcmp("sa", strtok(query_answer, " "))) {
				puts("Logical ^");
			}
			// only one word to be searched
			else {
				readIndex(index, tree->root);
			}
			readIndex(index,tree->root);
		}
	}
	
	free(tree->root);
	destroyList(list->head);
	free(tree);
	free(list);
 	return 0;
}
