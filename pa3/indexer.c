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
	Hash_Table * table = malloc(sizeof(Hash_Table));
	table->head = NULL;
	table->ptr = NULL;
	return table;
}

void insertHashTable(Hash_Table * table, char c, char * filename) {
	File_Node * fileptr = NULL;
	int index = 0;
	bool isFound;

	if (table->head == NULL) {
		table->head = malloc(sizeof(Prefix_Node));
		table->ptr = table->head;
	}
	index = hash(c);

	//invalid character
	if (index == -1) {
		//if at root
		if (table->ptr == table->head) return;
		else {
			table->ptr->isWord = true;
			for (fileptr = table->ptr->head; fileptr != NULL; fileptr = fileptr->next) {
				// compares files in File Linked List to current file
				if (strcmp(fileptr->filename, filename) == 0) {
					fileptr->occurrences++;
				}
			}
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
	char * token[100];
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

void recursivePrintTree(

void printTree(Prefix_Node * head) {
	char * buffer[100];
	Prefix_Node * ptr = head;
	

}

void dump_to_file(Hash_Table * inv_index, char * filename) {
	FILE * file = fopen(filename, "w");
	for (int i = 36; i > 0; i--) {
		
	}

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
