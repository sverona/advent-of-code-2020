#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv) {
	if (argc != 2) {
		fprintf(stderr, "usage: %s input\n", argv[0]);
		exit(1);
	}

	FILE* in = fopen(argv[1], "r");

	char* line = calloc(100, sizeof(char));
	char* token = calloc(10, sizeof(char));
	int i = 0;
	int bounds[20][4];

	int sum_of_invalid_values = 0;

	while (fgets(line, 100, in) != NULL) {
		if (i < 20) {
			char *after_colon = index(line, ':') + 2;
			int lower1, upper1, lower2, upper2;
			int success = sscanf(after_colon, "%d-%d or %d-%d",
					     &lower1, &upper1, &lower2, &upper2);

			if (success != 4) {
				fprintf(stderr, "error parsing %s", line);
				exit(1);
			}

			printf("%d %d %d %d\n", lower1, upper1, lower2, upper2);

			bounds[i][0] = lower1;
			bounds[i][1] = upper1;
			bounds[i][2] = lower2;
			bounds[i][3] = upper2;
		}

		if (i >= 25) {
			int j = 0;
			token = strtok(line, ",");
			do {
				int value = atoi(token);
				int this_value_good = 0;
				int k;
				for (k = 0; k < 20; k++) {
					if ((bounds[k][0] <= value && value <= bounds[k][1]) ||
					    (bounds[k][2] <= value && value <= bounds[k][3])) {
						this_value_good = 1;
					}
				}

				if (!this_value_good) {
					printf("invalid %d\n", value);
					sum_of_invalid_values += value;
				}

				j++;
			} while ((token = strtok(NULL, ",")) != NULL);
		}

		i++;
	}
	printf("SUM OF INVALIDS %d\n", sum_of_invalid_values);

	free(line);

	return 0;
}
