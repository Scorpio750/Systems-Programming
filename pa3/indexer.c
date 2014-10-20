#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>
#include <sys/stat.h>
#include "indexer.h"

Hash_Table * createTable () {
	Hash_Table * table = malloc(sizeof(Hash_Table));
	table->head = NULL;
	table->ptr = NULL;
	return table;
}

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

void recurseDir(char * dirname) {
	if (checkDir(dirname)) {
		DIR * dirp = opendir(dirname);
		struct dirent * entry = readdir(dirp);
		recurseDir(entry->d_name);
	}
	else { // file
		readFile();
	}
}

void insertHashTable(Hash_Table inv-index, char * word, char * filename) {
	Prefix_Node * temp = 
	FILE file = fopen(filename, r);

	if (inv-index->head == NULL) {
		inv-index->head = file;
	}

	for (int i = word[0]; i < strlen(word); i++) {
		hash(word[i]);
	}
}

int hash(char c) {
	char * index = strchr(acceptable, c);
	return (int)(index - acceptable);
}

void dump_to_file(Hash_Table * inv-index, char * filename) {
	FILE file = fopen(filename, w);
	
}

int main(int argc, char ** argv) {
	
	char * path = argv[1], * dirname = argv[2];
	int file_descr;
	Hash_Table * inv-index = createTable();

	if (argc != 3) {
		printf("Invalid number of arguments");
		return 0;
	}

	// make a new file 
	if ((file_descr = open(path, O_WRONLY)) == -1) {
		printf(stderr, "Unable to create file %s", path);
	}

	recurseDir(dirname);
	
	dump_to_file(Hash_Table * inv-index, char * filename);

	return 0;
}
