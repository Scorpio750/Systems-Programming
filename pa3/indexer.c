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

void insertHashTable(Hash_Table * inv_index, char c, char * filename) {
	int index = 0;

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
			for (ptr = table->ptr->head; ptr != NULL; ptr = ptr->next) {

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

void readFile(Hash_Table, inv_index, char * path) {
	FILE * filep = fopen(path, "r");
	char c;
	char * token[100];
	int acceptcount = 0;


	// converts mixed-case to lower-case and tokenizes
	while (c != EOF) {
		c = fgetc(filep);
		tolower(c);
		insertHashTable(inv_index, c, path);
	}
}

void recurseDir(Hash_Table inv_index, char * dirname) {
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
				recurseDir(buffer);
				free(buffer);
			}
			if (checkFile(entry->d_name)) {
				buffer = calloc(strlen(dirname) + strlen(entry->d_name) + 1, sizeof(char));
				strcat(buffer, dirname);
				strcat(buffer, entry->d_name);
				readFile(buffer);
				free(buffer);
			}
		}

		recurseDir(entry->d_name);
	}
}


void dump_to_file(Hash_Table * inv_index, char * filename) {
	FILE * file = fopen(filename, "w");
	
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
	
	dump_to_file(Hash_Table * inv_index, char * filename);

	return 0;
}
