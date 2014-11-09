/*
 * Alison Wong
 * Patrick Wu
 */
#include "search.h"

TNode *createNode(char c){
	TNode *node = (TNode *)malloc(sizeof(TNode));
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
			}
			else{
				//printf("Added to Linked List\n");
				llptr = addList(llptr, buffer);
			}
		}
	}while(i != EOF);
	//printf("DOES THIS EVER PRINT?\n");
	fclose(file);
	//printf("IS FCLOSE FAILING?\n");
}

<<<<<<< HEAD
 int main (int argc, char **argv){
	char * query_answer = malloc(256 * sizeof(char) + 1);
=======
int main (int argc, char **argv){
	char * query_response = malloc(256 * sizeof(char) + 1);
>>>>>>> 360935799be19c15949e87bbadb91aa36b1b69ae

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
	while (1) {
		puts("Enter your query:");
		scanf("%s", query_answer);
		if (!strcmp(query_answer,"q")) {
			puts("Exiting program");
			exit(1);
		}
		else {
			tokenize(query_answer);
			
			if (!strncmp("so ", query_answer, 3)) {

			}
			else if (!strncmp("sa ", query_answer, 3)) {

			}
			// only one word to be searched
			else {
				readIndex(index, tree);
			}
			readIndex(index,tree);
		}
	}
	free(tree);
 }
=======
 	readIndex(index, tree->root);

	// options
	/*
	while (true) {
		puts("Enter your query:");
		query_response = scanf();
		if (query_response = "q") {
			puts("Exiting program");
			exit(1);
		}
	}
	*/
 	//free(tree);
 	return 0;
}
>>>>>>> 360935799be19c15949e87bbadb91aa36b1b69ae
