#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>


/* checks to see if the file passed in is a file */ 
int checkDir(char * file) {
	struct stat statbuff;
	stat(file, &statbuff);
	return S_ISDIR(statbuff.st_mode);
}
/* checks to see if file is a file */
int checkFile(char * file) {
	struct stat statbuff;
	stat(file, &statbuff);
	return S_ISREG(statbuff.st_mode);
}

int main(int argc, char ** argv) {
	
	char * path;

	if (argc != 3) {
		printf("Invalid number of arguments");
		return 0;
	}

	path = argv[1];

	// make a new file 
	creat(path, 0644);
	
	if (checkFile(argv[2])) {
		
	}

	return 0;
}
