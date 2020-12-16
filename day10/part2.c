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
	}


	long *arrangements = calloc(line_count, sizeof(long));
	int start_index;
	for (start_index = line_count - 1; start_index >= 0; start_index--) {
		if (start_index == line_count - 1) {
			arrangements[start_index] = 1;
		} else {
			for (i = 1; i <= 3; i++) {
				if (start_index + i >= line_count) {
					continue;
				}
				int difference = sorted_voltages[start_index + i] - sorted_voltages[start_index];
				if (difference <= 3 && difference > 0) {
					arrangements[start_index] += arrangements[start_index + i];
				}
			}
		}
		printf("%d %ld\n", start_index, arrangements[start_index]);
	}

	long total_arrangements = 0;
	for (i = 0; i < 3; i++) {
		total_arrangements += (sorted_voltages[i] <= 3) * arrangements[i];
	}

	printf("%ld\n", total_arrangements);


	free(arrangements);
	free(sorted_voltages);
	free(voltages);
	free(line);

	return 0;
}
