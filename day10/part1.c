#include <stdio.h>
#include <stdlib.h>

int main(int argv, char** argc) {
	FILE* in = fopen(argc[1], "r");

	char* line = calloc(100, sizeof(char));
	int line_count = 0;
	while (fgets(line, 100, in) != NULL) {
		line_count++;
	}

	rewind(in);
	int* voltages = calloc(line_count, sizeof(int));
	int this_line = 0;
	while (fgets(line, 100, in) != NULL) {
		voltages[this_line++] = atoi(line);
	}

	int* sorted_voltages = calloc(line_count, sizeof(int));
	// Selection sort, because I am lazy.
	int i, j;
	int last_voltage = 0;
	for (i = 0; i < line_count; i++) {
		int min = 1 << 10;
		for (j = 0; j < line_count; j++) {
			if ((voltages[j] < min) && (voltages[j] > last_voltage)) {
				min = voltages[j];
			}
		}
		last_voltage = min;
		sorted_voltages[i] = min;
		printf("%d %d\n", i, sorted_voltages[i]);
	}

	int ones = (sorted_voltages[0] == 1);
	int threes = 1;

	for (i = 1; i < line_count; i++) {
		switch (sorted_voltages[i] - sorted_voltages[i - 1]) {
		case 1:
			ones++;
			break;
		case 3:
			threes++;
			break;
		}
	}

	printf("%d\n", ones * threes);

	free(sorted_voltages);
	free(voltages);
	free(line);

	return 0;
}
