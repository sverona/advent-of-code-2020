#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char next_gen(int slices, int rows, int cols, char current[][rows][cols], int slice, int row, int col) {
	char this_gen = current[slice][row][col];
	int ds, dr, dc;
	int neighbors = 0;
	for (ds = -1; ds <= 1; ds++) {
		for (dr = -1; dr <= 1; dr++) {
			for (dc = -1; dc <= 1; dc++) {
				int not_this_cell = (dr != 0) || (dc != 0) || (ds != 0);
				int row_valid = ((row + dr) >= 0) && ((row + dr) < rows);
				int col_valid = ((col + dc) >= 0) && ((col + dc) < cols);
				int slice_valid = ((slice + ds) >= 0) && ((slice + ds) < slices);
				if (slice_valid && row_valid && col_valid && not_this_cell) {
					// printf("   Checking neighbor s:%d r:%d c:%d.\n", slice + ds, row + dr, col + dc);
					neighbors += (current[slice + ds][row + dr][col + dc] == '#');
				}
			}
		}
	}


	int on = 0;
	if (this_gen == '#') {
		on = (neighbors == 2) || (neighbors == 3);
	} else {
		on = (neighbors == 3);
	}


	return on ? '#' : '.';
}


int main(int argc, char** argv) {
	if (argc != 3) {
		fprintf(stderr, "usage: %s input steps\n", argv[0]);
		exit(1);
	}

	FILE* in = fopen(argv[1], "r");

	int max_steps = atoi(argv[2]);

	char* line = calloc(100, sizeof(char));
	int xmax = 0;
	int ymax = 0;
	while (fgets(line, 100, in) != NULL) {
		int x = strlen(line);
		xmax = (x > xmax) ? x : xmax;
		ymax++;
	}

	int rows = max_steps + ymax + max_steps;
	int cols = max_steps + xmax + max_steps;
	int slices = max_steps + 1 + max_steps;
	char (*board)[rows][cols] = calloc(slices, sizeof(*board));
	char (*next)[rows][cols] = calloc(slices, sizeof(*next));

	rewind(in);
	int row = 0;
	int col;
	while (fgets(line, 100, in) != NULL) {
		for (col = 0; col < strlen(line); col++) {
			board[max_steps + 0][max_steps + row][max_steps + col] = line[col];
		}
		row++;
	}

	int slice;
	int t;
	for (t = 0; t < max_steps; t++) {
		int population = 0;
		for (slice = 0; slice < slices; slice++) {
			for (row = 0; row < rows; row++) {
				for (col = 0; col < cols; col++) {
					next[slice][row][col] = next_gen(slices, rows, cols, board, slice, row, col);
					population += (next[slice][row][col] == '#');
				}
			}
		}
		char (*aux)[rows][cols] = board;
		board = next;
		next = aux;
		printf("Generation %d has population %d.\n", t + 1, population);
	}

	free(next);
	free(board);
	free(line);

	return 0;
}
