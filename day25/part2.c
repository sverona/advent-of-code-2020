#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	if (argc != 2) {
		fprintf(stderr, "usage: %s input\n", argv[0]);
		exit(1);
	}

	FILE* in = fopen(argv[1], "r");

	char* line = calloc(100, sizeof(char));
	while (fgets(line, 100, in) != NULL) {
		printf("%s\n", line);
	}

	free(line);

	return 0;
}
