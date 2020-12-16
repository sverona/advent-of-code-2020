#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int terminates(char **lines, int total_lines, int switched) {
	int idx = 0;
	int acc = 0;

	int *seen = calloc(total_lines, sizeof(int));
	char *token = calloc(20, sizeof(char));
	char *copy = calloc(100, sizeof(char));

	while (0 <= idx && idx < total_lines) {
		if (seen[idx]) {
			return 0;
		}
		seen[idx]++;
		strcpy(copy, lines[idx]);
		token = strtok(copy, " \n");
		if ((strcmp(token, "nop") == 0) ||
		    ((idx == switched) && (strcmp(token, "jmp") == 0))) {
			idx++;
		} else if (strcmp(token, "acc") == 0) {
			token = strtok(NULL, " \n");
			acc += atoi(token);
			idx++;
		} else if ((strcmp(token, "jmp") == 0) ||
		    ((idx == switched) && (strcmp(token, "nop") == 0))) {
			token = strtok(NULL, " \n");
			idx += atoi(token);
		}
	}

	// free(seen);
	// free(token);
	// free(copy);
	return idx >= total_lines;
}

int run(char **lines, int total_lines, int switched) {
	int idx = 0;
	int acc = 0;

	int *seen = calloc(total_lines, sizeof(int));
	char *token = calloc(20, sizeof(char));
	char *copy = calloc(100, sizeof(char));


	while (0 <= idx && idx < total_lines) {
		if (seen[idx]) {
			return 0;
		}
		seen[idx]++;
		strcpy(copy, lines[idx]);
		token = strtok(copy, " \n");
		if ((strcmp(token, "nop") == 0) ||
		    ((idx == switched) && (strcmp(token, "jmp") == 0))) {
			idx++;
		} else if (strcmp(token, "acc") == 0) {
			token = strtok(NULL, " \n");
			acc += atoi(token);
			idx++;
		} else if ((strcmp(token, "jmp") == 0) ||
		    ((idx == switched) && (strcmp(token, "nop") == 0))) {
			token = strtok(NULL, " \n");
			idx += atoi(token);
		}
	}
	// free(seen);
	// free(token);
	// free(copy);
	return acc;
}

int main(int argv, char** argc) {
	FILE* in = fopen(argc[1], "r");

	char* line = calloc(100, sizeof(char));
	int total_lines = 0;
	while (fgets(line, 100, in) != NULL) {
		total_lines++;
		// printf("%s", line);
	}

	char **lines = calloc(total_lines, sizeof(*lines));
	rewind(in);
	int line_idx = 0;
	while (fgets(line, 100, in) != NULL) {
		char *this_line = calloc(100, sizeof(char));
		strcpy(this_line, line);
		lines[line_idx] = this_line;
		line_idx++;
	}

	int switch_idx;
	for (switch_idx = 0; switch_idx < total_lines; switch_idx++) {
		if (terminates(lines, total_lines, switch_idx)) {
			printf("%d %d\n", switch_idx, run(lines, total_lines, switch_idx));
		}
	}

	free(line);
	free(lines);

	return 0;
}
