#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {

	execlp("sh", "sh", "-c", argv[1], (char*)0);
	return 0;
}
