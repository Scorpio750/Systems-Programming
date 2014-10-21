#include "indexer.h"

/* Hash Table functions */
Hash_Table * createTable () {
	Hash_Table * inv_index = malloc(sizeof(Hash_Table));
	inv_index->head = NULL;
	inv_index->ptr = NULL;
	return inv_index;
}

// void insertHashTable(Hash_Table * inv_index, char c, char * filename) {

Prefix_Node *createNode(char c) {
	Prefix_Node *node = (Prefix_Node*)(calloc(1, sizeof(Prefix_Node)));
	node->c = c;
	node->depth = 0;
	node->isWord = false;
	node->head = NULL;
	node->next = (Prefix_Node**)(calloc(36, sizeof(Prefix_Node)));
	return node;
}

File_Node *createFileNode(char *filename) {
	if (filename == NULL) {
		return NULL;
	}

	File_Node *newnode = (File_Node*)calloc(1, sizeof(File_Node));
	newnode->filename = filename;
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
void checkList(Prefix_Node *node, char *filename){
	File_Node *fileNode;
	int prevOcc;
	if (node->head == NULL){
		fileNode = createFileNode(filename);
		node->head = fileNode;
		return;
	}

	for(fileNode = node->head; fileNode != NULL; fileNode = fileNode->next){
		if(fileNode->filename == filename){
			fileNode->occurrences++;
			if(fileNode->prev != NULL){
				prevOcc = fileNode->prev->occurrences;
				if(prevOcc < fileNode->occurrences)
					swap(node, fileNode->prev, fileNode);
			}
			return;
		}

		if(fileNode->next == NULL){
			File_Node *newNode = createFileNode(filename);
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

void insertTrie(FILE *file, Hash_Table *table, char *filename){
	if(table->head == NULL) {
		table->head = createNode('\0');
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
			if(ptr->isWord)
				checkList(ptr,filename);
			ptr = table->head;
		}
		c = tolower(fgetc(file));
	}
	return;
}

/*
void insertHashTable(Hash_Table * table, char c, char * filename) {
>>>>>>> efc9f486ea43b4dc04ea559c5da3298eaa9671bd
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


<<<<<<< HEAD
int hash(char c) {
	char * index = strchr(accepinv_index, c);
	if (index == NULL) {
		return -1;
	}
	return (int)(index - accepinv_index);
}
=======
>>>>>>> efc9f486ea43b4dc04ea559c5da3298eaa9671bd */

/* File System functions */
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
<<<<<<< HEAD
}-
*/

void recurseDir(Hash_Table * inv_index, char * dirname) {
	if (checkDir(dirname)) {
		char * buffer = NULL;
		DIR * dirp = opendir(dirname);
		struct dirent * entry;
		if (!dirp) {
			return;
		}

		while ((entry = readdir(dirp)) != NULL) {
			if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
				continue;
			}

			/* creates a buffer that appends found directory to current path in buffer
			 * then recurses with buffer as the new path */
			if (checkDir(entry->d_name)) { 
				buffer = calloc(strlen(dirname) + strlen(entry->d_name) + 1, sizeof(char));
				strcat(buffer, dirname);
				strcat(buffer, entry->d_name);
				recurseDir(inv_index, buffer);
				free(buffer);
			}
			if (checkFile(entry->d_name)) {
				buffer = calloc(strlen(dirname) + strlen(entry->d_name) + 1, sizeof(char));
				strcat(buffer, dirname);
				strcat(buffer, entry->d_name);

				FILE * filep = fopen(dirname, "r");
				insertTrie(filep, inv_index, dirname);
				free(buffer);
			}
		}

		recurseDir(inv_index, entry->d_name);
		free(dirp);
	}
}

/* Output functions */
bool isEmpty(Prefix_Node ** ptr) {
	
	for (int i = 0; i < 35; i++) {
		if (ptr[i] != NULL) return false;
	}
	return true;
}

char * formatOutput(char * buffer, File_Node * head) {
	File_Node *ptr;
	char *formatted_string = malloc((sizeof(char) * 2048) + 1);
	char *filename_list = malloc((sizeof(char) * 100) + 1);;
	
	sprintf(formatted_string, "<list> %s\n ", buffer);

	for (ptr = head; ptr != NULL; ptr = ptr->next) {
		sprintf(filename_list,"%s %d ", ptr->filename, ptr->occurrences);
		strcat(formatted_string, filename_list);
	}
	strcat(formatted_string, "\n</list>\n");
	return formatted_string;
	
}

void recursivePrintTree(char *buffer, Prefix_Node * ptr, FILE *file){
	int index;

	bool is_empty = isEmpty(ptr->next);
	if (is_empty) return;

	for (int i = 35; i > 0; i--) {
		if (ptr->next[i] == NULL)
			continue;
		index = ptr->next[i]->depth-1;
		buffer[index] = (char)ptr->next[i]->c;
		if(ptr->next[i]->isWord){
			buffer[index+1] = '\0';
			fputs(formatOutput(buffer,ptr->next[i]->head),file);
		}
		recursivePrintTree(buffer, ptr->next[i], file);
	}
}

void printTree(Prefix_Node * head, FILE *file) {
	char buffer[256];
	Prefix_Node * headptr = head;
	recursivePrintTree(buffer, headptr, file);
}

void dump_to_file(Hash_Table * inv_index, char * filename) {
	FILE * file = fopen(filename, "w");
	printTree(inv_index->head, file);
}

/* Free functions */
void destroyList(File_Node *head) {
	if(head == NULL)
		return;
	destroyList(head->next);
	free(head);
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
	dump_to_file(inv_index, path);
	return 0;
}
