#include <stdio.h>
#include <stdlib.h>

int main(int argv, char** argc) {
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

	int i, j;
	for (i = 0; i < lines; i++) {
		for (j = 0; j < i; j++) {
			if (nums[i] + nums[j] == 2020) {
				printf("%d %d %d\n", nums[i], nums[j], nums[i] * nums[j]);
			}
		}
	}

	free(nums);

	return 0;
}
