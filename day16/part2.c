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

	int fields_count = 20;
	int i = 0, j = 0, k = 0;

	int tickets_count = 0;
	while (fgets(line, 100, in) != NULL) {
		if (i == fields_count + 2 || i >= fields_count + 5) {
			tickets_count++;
		}
		i++;
	}

	rewind(in);

	char* token = calloc(10, sizeof(char));
	int** bounds = calloc(fields_count, sizeof(*bounds));

	int** tickets = calloc(tickets_count, sizeof(*tickets));
	int* good = calloc(tickets_count, sizeof(int));
	int ticket_idx = 0;

	i = 0;
	while (fgets(line, 100, in) != NULL) {
		if (i < fields_count) {
			char *after_colon = index(line, ':') + 2;
			int lower1, upper1, lower2, upper2;
			int success = sscanf(after_colon, "%d-%d or %d-%d",
					     &lower1, &upper1, &lower2, &upper2);

			if (success != 4) {
				fprintf(stderr, "error parsing %s", line);
				exit(1);
			}

			bounds[i] = calloc(4, sizeof(int));

			bounds[i][0] = lower1;
			bounds[i][1] = upper1;
			bounds[i][2] = lower2;
			bounds[i][3] = upper2;
		}

		if (i == fields_count + 2 || i >= fields_count + 5) {
			tickets[ticket_idx] = calloc(fields_count, sizeof(int));
			j = 0;
			token = strtok(line, ",");
			int this_ticket_good = 1;
			do {
				int value = atoi(token);
				int this_value_good = 0;
				for (k = 0; k < fields_count; k++) {
					if ((bounds[k][0] <= value && value <= bounds[k][1]) ||
					    (bounds[k][2] <= value && value <= bounds[k][3])) {
						this_value_good = 1;
					}
				}

				if (!this_value_good) {
					this_ticket_good = 0;
				}

				tickets[ticket_idx][j++] = value;
			} while ((token = strtok(NULL, ",")) != NULL);

			good[ticket_idx++] = this_ticket_good;
		}
		i++;
	}

	int* matches = calloc(fields_count, sizeof(int));
	int field, position;
	for (position = 0; position < fields_count; position++) {
		for (field = 0; field < fields_count; field++) {
			int matched = 1;
			for (ticket_idx = 0; ticket_idx < tickets_count; ticket_idx++) {
				int this_value = tickets[ticket_idx][field];
				if (!good[ticket_idx]) {
					continue;
				}
				if ((bounds[position][0] <= this_value && this_value <= bounds[position][1]) ||
				    (bounds[position][2] <= this_value && this_value <= bounds[position][3])) {
				} else {
					matched = 0;
				}
			}
			if (matched) {
				printf("Field %d matches position %d\n", field, position);
				matches[position]++;
			}
		}
	}

	int* positions = calloc(fields_count, sizeof(int));
	for (position = 0; position < fields_count; position++) {
		printf("%d %d\n", position, matches[position]);
		positions[position] = -1;
	}

	int max_count = 1;
	for (max_count = 1; max_count <= fields_count; max_count++) {
		for (position = 0; position < fields_count; position++) {
			if (matches[position] > max_count) {
				continue;
			}
			for (field = 0; field < fields_count; field++) {
				int matched = 1;
				for (ticket_idx = 0; ticket_idx < tickets_count; ticket_idx++) {
					int this_value = tickets[ticket_idx][field];
					if (!good[ticket_idx]) {
						continue;
					}
					if ((bounds[position][0] <= this_value && this_value <= bounds[position][1]) ||
					    (bounds[position][2] <= this_value && this_value <= bounds[position][3])) {
					} else {
						matched = 0;
					}
				}
				if (matched) {
					if (positions[field] < 0) {
						printf("Matched field %d to position %d\n", field, position);
						positions[field] = position;
					}
				}
			}
		}
	}

	long product = 1L;
	for (i = 0; i < fields_count; i++) {
		if (positions[i] < 6) {
			// printf("%d %d %d\n", i, positions[i], tickets[0][positions[i]]);
			product *= tickets[0][i];
		}
	}

	printf("%ld\n", product);

	free(matches);
	free(positions);
	free(good);
	free(bounds);
	free(tickets);
	free(token);
	free(line);

	return 0;
}
