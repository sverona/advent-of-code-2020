#include <stdio.h>
#include <stdlib.h>

int main(int argv, char** argc) {
	if (argv != 2) {
		fprintf(stderr, "usage: %s [input]\n", argc[0]);
		exit(1);
	}

	FILE* in = fopen(argc[1], "r");

	int lines = 0;
	int num;
	while (fscanf(in, "%d", &num) != EOF) {
		lines++;
		printf("%d\n", num);
	}

	int* nums = calloc(lines, sizeof(int));

	rewind(in);
	int line = 0;
	while (fscanf(in, "%d", &num) != EOF) {
		nums[line] = num;
		printf("%d\n", num);
		line++;
	}

	int i, j, k;
	for (i = 0; i < lines; i++) {
		for (j = 0; j < i; j++) {
			for (k = 0; k < j; k++) {
				if (nums[i] + nums[j] + nums[k] == 2020) {
					printf("%d %d %d %d\n",
						nums[i], nums[j], nums[k],
						nums[i] * nums[j] * nums[k]);
				}
			}
		}
	}

	free(nums);

	return 0;
}
