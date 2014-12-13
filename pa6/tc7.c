#include "memory.c"

int main (int argc, char **argv) {
	char *test = calloc(50);
	test[0] = 'c';
	test[1] = 'a';
	test[2] = 't';
	test[4] = 's';
	printf("%s\n", test);
	free(test);
	return 0;
}
