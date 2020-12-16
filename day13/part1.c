#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv) {
	if (argc != 2) {
		fprintf(stderr, "usage: %s input\n", argv[0]);
		exit(1);
	}

	FILE* in = fopen(argv[1], "r");

	char* line = calloc(300, sizeof(char));
	char* token = calloc(10, sizeof(char));
	int i = 0;
	int time_index = 0;
	int bus_count = 0;
	while (fgets(line, 300, in) != NULL) {
		if (i++ == 0) {
			time_index = atoi(line);
		} else {
			token = strtok(line, ",");
			do {
				if (token[0] == 'x') {
					continue;
				} else {
					int this_bus = atoi(token);
					int wait = this_bus - (time_index % this_bus);
					printf("time:%d bus:%d wait:%d times:%d\n", time_index, this_bus, wait, this_bus * wait);
				}
			} while ((token = strtok(NULL, ",")) != NULL);
		}
	}

	free(line);
	free(token);

	return 0;
}
