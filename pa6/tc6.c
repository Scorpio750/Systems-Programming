#include "memory.c"

int main (int argc, char **argv) {
	char *test = malloc(123456);
	char *test2 = malloc(123456);
	atexit(leak_detection);
	return 0;
} 
