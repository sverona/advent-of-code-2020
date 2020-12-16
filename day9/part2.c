#include <stdio.h>
#include <stdlib.h>

int main(int argv, char** argc) {
	FILE* in = fopen(argc[1], "r");

	char* line = calloc(100, sizeof(char));
	int line_count = 0;

	while (fgets(line, 100, in) != NULL) {
		line_count++;
	}

	int *numbers = calloc(line_count, sizeof(int));

	rewind(in);
	int this_line = 0;
	int num = 0;
	while (fgets(line, 100, in) != NULL) {
		num = atoi(line);
		if (this_line >= 25) {
			int i, j;
			int bad = 1;
			for (i = 0; i < this_line; i++) {
				for (j = 0; j < i; j++) {
					if (numbers[i] + numbers[j] == num) {
						bad = 0;
					}
				}
			}

			if (bad) {
				break;
			}
		}
		numbers[this_line++] = num;
	}

	printf("BAD %d\n", num);
	int *sums = calloc(this_line, sizeof(int));
	int i, j;
	int argmin, argmax;
	for (i = 0; i < this_line; i++) {
		for (j = 0; (i + j) < this_line; j++) {
			sums[i] += numbers[i + j];
			if (sums[i] == num) {
				argmin = i;
				argmax = i + j;
			}
		}
	}


	int min = num, max = 0;

	for (i = argmin; i <= argmax; i++) {
		if (numbers[i] < min) {
			min = numbers[i];
		}
		if (numbers[i] > max) {
			max = numbers[i];
		}
	}

	printf("%d %d %d\n", min, max, min + max);


	free(line);

	return 0;
}
