#include "memory.c"

int main (int argc, char **argv) {
	char *test;
	test = test_func();
	printf("%s\n", test);
	free(test);
	return 0;
}
