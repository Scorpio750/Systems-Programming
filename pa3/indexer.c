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
	//printf("createFileNode: entering function\n");
	File_Node *newnode = (File_Node*)calloc(1, sizeof(File_Node));
	//newnode->pathname = pathname; //pointers dont work this way
	newnode->pathname = (char*)malloc((strlen(pathname)+1)*sizeof(char));
	strcpy(newnode->pathname, pathname);
	//printf("PATHNAME: %s, NODEPATH: %s\n", pathname, newnode->pathname);
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
 * a new fileNode head. Else, we traverse it to compare filenames */
void checkList(Prefix_Node *node, char *pathname){
	File_Node *fileNode;
	int prevOcc; // gives you # of nodes previous to the one you were at
	
	/* creates new fileNode if there isn't one already */
	if (node->head == NULL){
		//printf("PATHNAME: %s\n", pathname);
		//printf("checkList: starting to create a new filenode\n");
		fileNode = createFileNode(pathname);
		//printf("checkList: finished creating new filenode\n");
		//printf("PATHNAME: %s\n", fileNode->pathname);
		node->head = fileNode;
		return;
	}

	/* file comparator */
	for(fileNode = node->head; fileNode != NULL; fileNode = fileNode->next){
		printf("FILE 1: %s\t FILE 2:%s\n", fileNode->pathname, pathname);

		if(strcmp(fileNode->pathname, pathname) == 0) {
			printf("PATHNAME ---------------: [%s]\n", pathname);
			fileNode->occurrences++;
			if(fileNode->prev != NULL){
				prevOcc = fileNode->prev->occurrences;
				if(prevOcc < fileNode->occurrences)
					swap(node, fileNode->prev, fileNode);
			}
			return;
		}

		if(fileNode->next == NULL){
			//printf("PATHNAME: %s\n", pathname);
			//printf("checkList: caution! getting to unauthorized area\n");
			File_Node *newNode = createFileNode(pathname);
			//printf("checkList: left unauthorized area, phew\n");
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
	int index;
	int c = tolower(fgetc(file));
	//printf("DID WE EVEN GET HERE?\n");
	/*if (c == EOF){
		printf("c is EOF\n");
	}*/
	while (c != EOF){
		printf("THE VALUE OF POINTER IS [%c]\n", ptr->c);
		if (isalpha(c) || isdigit(c)){
			index = hash(c);
			if (ptr->next[index] == NULL){
				ptr->next[index] = createNode(c);
				//printf("CHARACTER THAT IS ADDED [%c] [%c]\n", c, ptr->next[index]->c );
				ptr->next[index]->depth  = ptr->depth+1;
			}
			ptr = ptr->next[index];
		}
		if ((!isalpha(c)) && (!isdigit(c)) && ptr != table->head){
			ptr->isWord = true;
			if (ptr->isWord) {
				printf("WE HAVE A WORD, LADIES AND GENTLEMEN\n");
				//char * filename = parseBuffer(pathname);
				printf("PATH: %s\n", pathname);
				checkList(ptr,pathname);
			}
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
	//printf("THE VALUE OF FILENAME IS [%s]\n", filename);
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
		//printf("ABOUT TO ENTER TRIE\n");
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
			//printf("AND THE VALUE OF %s IS %d\n", buffer, checkDir((char*)entry->d_name));
			if (checkDir(buffer)) { 
				//printf("BUFFER >>> %s\n ENTRY >>> %s\n", buffer, entry->d_name);

				recurseDir(inv_index, buffer);
				free(buffer);
			}
			if (checkFile(buffer)) {
				//printf("ABOUT TO ENTER PARSER\n");
				filep = fopen(buffer, "r");
				if (filep == NULL) {
					printf("STHISEFSJAFJDKA\n");
					exit(1);
				}
				printf("\nENTERING INSERT TRIE WITH %s\n", buffer);
				insertTrie(filep, inv_index, buffer);
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
	char *filename_list = malloc((sizeof(char) * 100) + 1);
	char *filename = malloc((sizeof(char) * 100) + 1);
	sprintf(formatted_string, "<list> %s\n ", buffer);

	for (ptr = head; ptr != NULL; ptr = ptr->next) {
		filename = parseBuffer(ptr->pathname);
		sprintf(filename_list,"%s %d ", filename, ptr->occurrences);
		//sprintf(filename_list,"%s %d ", ptr->pathname, ptr->occurrences);
		//printf("PATHNAME: %s\n", ptr->pathname);
		strcat(formatted_string, filename_list);
	}
	strcat(formatted_string, "\n</list>\n");
	free(filename_list);
	free(filename);
	return formatted_string;
}

void recursivePrintTree(char *buffer, Prefix_Node * ptr, FILE *file){
	int index;
	char *formatted_string;
	if (ptr == NULL) {
		printf("Invalid file.\n");
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
		printf("Invalid number of arguments.\nindex <write-file> <directory>\n");
		return 0;
	}

	// make a new file 
	if ((file_descr = open(path, O_WRONLY)) == -1) {
		printf("Unable to create file %s\n", path);
	}

	recurseDir(inv_index, dirname);
	//printf("MEOW\n");
	dump_to_file(inv_index, path);
	free(inv_index);
	return 0;
}
