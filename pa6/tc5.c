#include "memory.c"

int main (int argc, char **argv) {
	char *test = malloc (1000000);
	char *test2 = malloc(55520);
	free(test);
	test2 = malloc(55520);
	free(test2);
	return 0;
}
