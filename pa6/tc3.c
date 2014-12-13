#include "memory.c"

int main (int argc, char **argv){
	char *test;
	test = test_func();
	free(test);
	free(test);
	return 0;
}
