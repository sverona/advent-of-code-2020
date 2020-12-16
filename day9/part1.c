#include <stdio.h>
#include <stdlib.h>

int main(int argv, char** argc) {
	FILE* in = fopen(argc[1], "r");

	char* line = calloc(100, sizeof(char));
	int line_count = 0;

	int *numbers = calloc(25, sizeof(int));

	while (fgets(line, 100, in) != NULL) {
		int num = atoi(line);
		int bad = 1;
		if (line_count >= 25) {
			int i, j;
			for (i = 0; i < 25; i++) {
				for (j = 0; j < 25; j++) {
					if (numbers[i] + numbers[j] == num) {
						bad = 0;
					}
				}
			}
			if (bad) {
				printf("BAD %d\n", num);
				break;
			}
		}
		numbers[line_count % 25] = num;
		// printf("%s\n", line);
		line_count++;
	}


	free(numbers);
	free(line);

	return 0;
}
