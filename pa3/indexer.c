#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>
#include <ctype.h>
#include <sys/stat.h>
#include "indexer.h"

/* Hash Table functions */
Hash_Table * createTable () {
	Hash_Table * inv_index = malloc(sizeof(Hash_Table));
	inv_index->head = NULL;
	inv_index->ptr = NULL;
	return inv_index;
}

void insertHashTable(Hash_Table * inv_index, char c, char * filename) {
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
}

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
				readFile(inv_index,buffer);
				free(buffer);
			}
		}

		recurseDir(inv_index, entry->d_name);
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
		index = ptr->next[i]->depth;
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
