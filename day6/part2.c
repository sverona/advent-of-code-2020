#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argv, char** argc) {
	FILE* in = fopen(argc[1], "r");

	char* line = calloc(100, sizeof(char));
	int* answers = calloc(26, sizeof(int));
	int total = 0;
	int group_size = 0;
	while (fgets(line, 100, in) != NULL) {
		printf("%s", line);
		if (line[0] == '\n') {
			int i;
			for (i = 0; i < 26; i++) {
				if (answers[i] == group_size) {
					printf(" %c", 'a' + i);
					total++;
				}
				answers[i] = 0;
			}
			printf("\n");
			group_size = 0;
		} else {
			group_size++;
			int i;
			for (i = 0; i < strlen(line); i++) {
				if (islower(line[i])) {
					int idx = line[i] - 'a';
					answers[idx]++;
				}
			}
		}
	}

	int i;
	for (i = 0; i < 26; i++) {
		if (answers[i] == group_size) {
			printf(" %c", 'a' + i);
			total++;
		}
		answers[i] = 0;
	}

	free(line);
	free(answers);
	printf("TOTAL %d\n", total);

	return 0;
}
