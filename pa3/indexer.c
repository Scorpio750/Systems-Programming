// Patrick Wu
// Alison Wong
// Team -__-
#include "indexer.h"

/* Hash Table functions */
Hash_Table * createTable () {
	Hash_Table * inv_index = malloc(sizeof(Hash_Table));
	inv_index->head = NULL;
	return inv_index;
}

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
	newnode->pathname = (char*)malloc((strlen(pathname)+1)*sizeof(char));
	strcpy(newnode->pathname, pathname);
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
		if(strcmp(fileNode->pathname, pathname) == 0) {
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
			return;
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
	int index;
	int c = tolower(fgetc(file));
	while (c != EOF){
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
			if (ptr->isWord) {
				checkList(ptr,pathname);
			}
			ptr = table->head;
		}
		c = tolower(fgetc(file));
	}
	return;
}

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

void recurseDir(Hash_Table * inv_index, char * dirname) {
	FILE * filep;

	// if dirname is a file
	if (checkFile(dirname)) {
		filep = fopen(dirname, "r");
		insertTrie(filep, inv_index, dirname);
		fclose(filep); 
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
			if (checkDir(buffer)) { 

				recurseDir(inv_index, buffer);
				free(buffer);
			}
			if (checkFile(buffer)) {
				filep = fopen(buffer, "r");
				if (filep == NULL) {
					fprintf(stderr, "File does not exist\n");
					fclose(filep);
					exit(1);
				}
				insertTrie(filep, inv_index, buffer);
				free(buffer);
				fclose(filep);
			}
		}
		closedir(dirp);
	}
	return;
}

/* Output functions */
bool isEmpty(Prefix_Node ** ptr) {
	int i;
	for (i = 0; i <= 35; i++) {
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
	int i;
	char *formatted_string;
	if (ptr == NULL) {
		fprintf(stderr, "File Does Not Exist\n");
		return;
	}
	bool is_empty = isEmpty(ptr->next);
	if (is_empty) return;

	for (i = 0; i <= 35; i++) {
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
	// recursively calls destroyList to free all fileNodes
	destroyList(head->next);
	free(head->pathname);
	free(head);
	return;
}

void destroyNode(Prefix_Node *node) {
	int i;
	if (node == NULL)
		return;
	for (i = 0; i < 36; i++){
		if (node->next[i] == NULL)
			continue;
		destroyNode(node->next[i]);
	}
	destroyList(node->head);
	free(node->next);
	free(node);
	return;
}

int main(int argc, char ** argv) {

	char * path = argv[1], * dirname = argv[2];
	char * ow_response = malloc(sizeof(char) * 10);
	bool file_exists = false;
	Hash_Table * inv_index = createTable();
	int i;

	if (argc != 3) {
		printf("Invalid number of arguments\n");
		return 0;
	}

	// check to see if specified inv-index filename is already in your current directory
	DIR * dirp = opendir(".");
	struct dirent * entry;
	if (!dirp) {
		return 0;
	}

	/* iterates through every file in a directory with readdir */
	while ((entry = readdir(dirp)) != NULL) {
		if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
			continue;
		}
		if (strcmp(entry->d_name, path) == 0) {
			file_exists = true;
		}
	}
	if (file_exists) {
		printf("File exists with the name %s.\nWould you like to overwrite it? (y/n)\n", path);
		scanf("%s", ow_response);
		for (i = 0; i < strlen(ow_response); i++) {
			ow_response[i] = tolower(ow_response[i]);
		}
		if (!strcmp(ow_response, "n") || !strcmp(ow_response, "no")) {
			printf("Aborting...\n");
			return 0;	
		}
	}

	recurseDir(inv_index, dirname);
	dump_to_file(inv_index, path);
	destroyNode(inv_index->head);

	// frees
	closedir(dirp);
	free(inv_index);
	free(ow_response);
	return 0;
}
