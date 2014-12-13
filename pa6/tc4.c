#include "memory.c"

int main (int argc, char **argv) {
	char *test = test_func();
	free(test+10);
	free(test);
	return 0;
}
