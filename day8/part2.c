#include <stdio.h>
#include <stdlib.h>

int main(int argv, char** argc) {
	FILE* in = fopen(argc[1], "r");

	char* line = calloc(100, sizeof(char));
	while (fgets(line, 100, &num) != NULL) {
		printf("%s\n", line);
	}

	free(line);

	return 0;
}
