#include <stdio.h>
#include <stdlib.h>

static void foo() {
	static int is_initialized = 0;
	printf("%d\n", is_initialized);
	if (!is_initialized) {
		printf("initializing stuff\n");
		is_initialized = 1;
		printf("%d\n", is_initialized);
		}
	printf("do stuff\n");
}

int main(int argc, char **argv) {
	foo();
	foo();
	foo();
}
