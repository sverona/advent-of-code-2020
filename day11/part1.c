#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char next_gen(int row, int col, char** current, int rows, int cols) {
	char this_gen = current[row][col];
	int dr, dc;
	int neighbors = 0;
	for (dr = -1; dr <= 1; dr++) {
		for (dc = -1; dc <= 1; dc++) {
			int not_this_seat = (dr != 0) || (dc != 0);
			int row_valid = ((row + dr) >= 0) && ((row + dr) < rows);
			int col_valid = ((col + dc) >= 0) && ((col + dc) < cols);
			if (row_valid && col_valid && not_this_seat) {
				neighbors += (current[row + dr][col + dc] == '#');
			}
		}
	}

	switch(this_gen) {
	case '.':
		return '.';
	break;
	case 'L':
		if (neighbors == 0) {
			return '#';
		} else {
			return 'L';
		}
	break;
	case '#':
		if (neighbors >= 4) {
			return 'L';
		} else {
			return '#';
		}
	break;
	}

	return '.';
}

int main(int argc, char** argv) {
	if (argc != 2) {
		fprintf(stderr, "usage: %s input\n", argv[0]);
		exit(1);
	}

	FILE* in = fopen(argv[1], "r");

	char* line = calloc(200, sizeof(char));
	int line_count = 0;
	while (fgets(line, 200, in) != NULL) {
		line_count++;
	}

	char** lines = calloc(line_count, sizeof(*lines));
	rewind(in);
	int i = 0;
	while (fgets(line, 200, in) != NULL) {
		lines[i] = calloc(strlen(line), sizeof(char));
		strcpy(lines[i], line);
		i++;
	}
	int line_length = strlen(lines[0]);

	char** next = calloc(line_count, sizeof(*lines));
	for (i = 0; i < line_count; i++) {
		next[i] = calloc(line_length, sizeof(char));
	}

	int gen = 0;
	int changes = 0;
	int occupied = 0;
	int row, col;
	do {
		changes = 0;
		occupied = 0;
		for (row = 0; row < line_count; row++) {
			for (col = 0; col < line_length; col++) {
				next[row][col] = next_gen(row, col, lines, line_count, line_length);
				changes += (next[row][col] != lines[row][col]);
				occupied += (next[row][col] == '#');
			}
		}
		char** aux = lines;
		lines = next;
		next = aux;
		gen++;
		printf("Generation %d occurred with %d changes and %d occupied.\n", gen, changes, occupied);
	} while (changes > 0);

	free(lines);
	free(line);

	return 0;
}
