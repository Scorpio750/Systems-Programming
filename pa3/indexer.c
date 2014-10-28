#include "indexer.h"

/* Hash Table functions */
Hash_Table * createTable () {
	Hash_Table * inv_index = malloc(sizeof(Hash_Table));
	inv_index->head = NULL;
	return inv_index;
}

// void insertHashTable(Hash_Table * inv_index, char c, char * filename) {

Prefix_Node *createNode(char c) {
	Prefix_Node *node = (Prefix_Node*)calloc(1, sizeof(Prefix_Node));
	node->c = c;
	node->depth = 0;
	node->isWord = false;
	node->head = NULL;
	node->next = (Prefix_Node**)calloc(36, sizeof(Prefix_Node));
	return node;
}

File_Node *createFileNode(char *pathname) {
	if (pathname == NULL) {
		return NULL;
	}

	File_Node *newnode = (File_Node*)calloc(1, sizeof(File_Node));
	newnode->pathname = pathname;
	newnode->occurrences = 1;
	newnode->next = NULL;
	newnode->prev = NULL;
	if (newnode != NULL) {
		return newnode;
	}
	else {
		return NULL;
	}
}


void swap(Prefix_Node *node, File_Node *small, File_Node *big){
	if(small == node->head) {
		small->next = big->next;
		small->prev = big;
		big->prev = NULL;
		big->next = small;
		node->head = big;
		return;
	}

	small->next = big->next;
	big->prev = small->prev;
	small->prev->next = big;
	small->prev = big;
	big->next = small;

	if(small->next != NULL)
		small->next->prev = small;

	int prevOcc;
	if(big->prev != NULL) {
		prevOcc = big->prev->occurrences;
		if(prevOcc < big->occurrences) {
			swap(node, big->prev, big);
		}
	}
	return;
}

/* checks to see if there is a linked list at that node. If there isn't, we create
 * a new fileNode head. Else, we traverse it to cmp filenames */
void checkList(Prefix_Node *node, char *pathname){
	File_Node *fileNode;
	int prevOcc; // gives you # of nodes previous to the one you were at
	if (node->head == NULL){
		fileNode = createFileNode(pathname);
		node->head = fileNode;
		return;
	}

	/* file comparator */
	for(fileNode = node->head; fileNode != NULL; fileNode = fileNode->next){
		if(fileNode->pathname == pathname){
			fileNode->occurrences++;
			if(fileNode->prev != NULL){
				prevOcc = fileNode->prev->occurrences;
				if(prevOcc < fileNode->occurrences)
					swap(node, fileNode->prev, fileNode);
			}
			return;
		}

		if(fileNode->next == NULL){
			File_Node *newNode = createFileNode(pathname);
			fileNode->next = newNode;
			newNode->next = NULL;
			newNode->prev = fileNode;
		}
	}
}

int hash(char c) {
	char * index = strchr(acceptable, c);
	if (index == NULL) {
		return -1;
	}
	return (int)(index - acceptable);
}

void insertTrie(FILE *file, Hash_Table *table, char *pathname){
	if(table->head == NULL) {
		table->head = createNode('-');
	}

	Prefix_Node *ptr = table->head;
	printf("THE VALUE OF POINTER IS %c\n", ptr->c);
	int index;
	int c = tolower(fgetc(file));


	while (c != EOF){
		printf("	CHAR READ IS %c\n", c);
		if (isalpha(c) || isdigit(c)){
			index = hash(c);
			if (ptr->next[index] == NULL){
				ptr->next[index] = createNode(c);
				ptr->next[index]->depth  = ptr->depth+1;
			}
			ptr = ptr->next[index];
		}
		if ((!isalpha(c)) && (!isdigit(c)) && ptr != table->head){
			ptr->isWord = true;
			if (ptr->isWord)
				checkList(ptr,pathname);
			ptr = table->head;
		}
		c = tolower(fgetc(file));
	}
	return;
}

/*
void insertHashTable(Hash_Table * table, char c, char * filename) {
	File_Node * fileptr = NULL;
	int index = 0;
	bool isFound;

	if (inv_index->head == NULL) {
		inv_index->head = malloc(sizeof(Prefix_Node));
		inv_index->ptr = inv_index->head;
	}
	index = hash(c);

	//invalid character
	if (index == -1) {
		//if at root
		if (inv_index->ptr == inv_index->head) return;
		else {
			inv_index->ptr->isWord = true;
			for (fileptr = inv_index->ptr->head; fileptr != NULL; fileptr = fileptr->next) {
				// compares files in File Linked List to current file
				if (strcmp(fileptr->filename, filename) == 0) {
					fileptr->occurrences++;
				}
			}
		}
	}
}

int hash(char c) {
	char * index = strchr(accepinv_index, c);
	if (index == NULL) {
		return -1;
	}
	return (int)(index - accepinv_index);
}
*/

/* File System functions */

/* checkDir and checkFile return 1 if it is a file/dir, 0 otherwise */
int checkDir(char * file) {
	struct stat statbuff;
	stat(file, &statbuff);
	return S_ISDIR(statbuff.st_mode);
}

int checkFile(char * file) {
	struct stat statbuff;
	stat(file, &statbuff);
	return S_ISREG(statbuff.st_mode);
}

/* parses buffer with strrchr */
char * parseBuffer(char * buffer) {
	char * filename = malloc(sizeof(char) * strlen(buffer) + 1);
	filename = strcpy(filename, strrchr(buffer, '/') + 1);
	printf("THE VALUE OF FILENAME IS %s\n", filename);
	return filename;
}

/*
 void readFile(Hash_Table * inv_index, char * path) {
	FILE * filep = fopen(path, "r");
	char c = '\0';
	char * token[256];
	int acceptcount = 0;

	// converts mixed-case to lower-case and tokenizes
	while (c != EOF) {
		c = fgetc(filep);
		tolower(c);
		insertHashTable(inv_index, c, path);
	}
}-
*/

void recurseDir(Hash_Table * inv_index, char * dirname) {
	FILE * filep;

	// if dirname is a file
	if (checkFile(dirname)) {
		filep = fopen(dirname, "r");
		printf("ABOUT TO ENTER TRIE\n");
		insertTrie(filep, inv_index, dirname);
		return;
	}
	else if (checkDir(dirname)) {
		char * buffer = NULL;
		DIR * dirp = opendir(dirname);
		struct dirent * entry;
		if (!dirp) {
			return;
		}
		
		/* iterates through every file in a directory with readdir */
		while ((entry = readdir(dirp)) != NULL) {
			if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
				continue;
			}

			/* creates a buffer that appends found directory to current path in buffer
			 * then recurses with buffer as the new path */
			buffer = calloc(strlen(dirname) + strlen(entry->d_name) + 2, sizeof(char));
			sprintf(buffer, "%s/%s", dirname, entry->d_name);
			printf("AND THE DIR VALUE OF %s IS %d\n", buffer, checkDir(buffer));
			if (checkDir(buffer)) { 
				printf("BUFFER >>> %s\n ENTRY >>> %s\n", buffer, entry->d_name);

				recurseDir(inv_index, buffer);
				free(buffer);
			}
			printf("AND THE FILE VALUE OF %s IS %d\n", buffer, checkFile(buffer));
			if (checkFile(buffer)) {
				printf("ABOUT TO ENTER PARSER\n");
				char * filename = parseBuffer(buffer);
				filep = fopen(filename, "r");
				printf("ENTERING INSERT TRIE\n");
				insertTrie(filep, inv_index, buffer);
				free(filename);
				free(buffer);
			}
		}

		// recurseDir(inv_index, entry->d_name);
		free(dirp);
	}
	return;
}

/* Output functions */
bool isEmpty(Prefix_Node ** ptr) {
	for (int i = 0; i < 35; i++) {
		if (ptr[i] != NULL) 
			return false;
	}
	return true;
}

char * formatOutput(char * buffer, File_Node * head, char *formatted_string) {
	File_Node *ptr;
	char *filename_list = malloc((sizeof(char) * 100) + 1);;
	
	sprintf(formatted_string, "<list> %s\n ", buffer);

	for (ptr = head; ptr != NULL; ptr = ptr->next) {
		sprintf(filename_list,"%s %d ", ptr->pathname, ptr->occurrences);
		strcat(formatted_string, filename_list);
	}
	strcat(formatted_string, "\n</list>\n");
	free(filename_list);
	return formatted_string;
}

void recursivePrintTree(char *buffer, Prefix_Node * ptr, FILE *file){
	int index;
	char *formatted_string;
	if (ptr == NULL) {
		printf("DAMNIt\n");
		return;
	}
	bool is_empty = isEmpty(ptr->next);
	if (is_empty) return;

	for (int i = 0; i < 35; i++) {
		if (ptr->next[i] == NULL)
			continue;
		index = ptr->next[i]->depth-1;
		buffer[index] = (char)ptr->next[i]->c;
		if(ptr->next[i]->isWord){
			buffer[index+1] = '\0';
			formatted_string = malloc((sizeof(char) * 2048) + 1);
			fputs(formatOutput(buffer,ptr->next[i]->head,formatted_string),file);
			free(formatted_string);
		}
		recursivePrintTree(buffer, ptr->next[i], file);
	}
	return;
}

void printTree(Prefix_Node * head, FILE * file) {
	char * buffer = calloc(256, sizeof(char));
	Prefix_Node * headptr = head;
	recursivePrintTree(buffer, headptr, file);
	free(buffer);
	return;
}

void dump_to_file(Hash_Table * inv_index, char * filename) {
	FILE * file = fopen(filename, "w");
	printTree(inv_index->head, file);
	fclose(file);
	return;
}

/* Free functions */
void destroyList(File_Node *head) {
	if(head == NULL)
		return;
	destroyList(head->next);
	free(head);
	return;
}

void destroyNode(Prefix_Node *node) {
	if (node == NULL)
		return;
	for (int i = 0; i < 36; i++){
		if (node->next[i] == NULL)
			continue;
		destroyNode(node->next[i]);
	}
	free (node->next);
	destroyList(node->head);
	free(node);
	return;
}

int main(int argc, char ** argv) {

	char * path = argv[1], * dirname = argv[2];
	int file_descr;
	Hash_Table * inv_index = createTable();

	if (argc != 3) {
		printf("Invalid number of arguments");
		return 0;
	}

	// make a new file 
	if ((file_descr = open(path, O_WRONLY)) == -1) {
		printf("Unable to create file %s", path);
	}

	recurseDir(inv_index, dirname);
	printf("MEOW\n");
	dump_to_file(inv_index, path);
	free(inv_index);
	return 0;
}
