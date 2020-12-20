#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char next_gen(int mslices, int slices, int rows, int cols, char current[][slices][rows][cols], int mslice, int slice, int row, int col) {
	char this_gen = current[mslice][slice][row][col];
	int dm, ds, dr, dc;
	int neighbors = 0;
	for (dm = -1; dm <= 1; dm++) {
		for (ds = -1; ds <= 1; ds++) {
			for (dr = -1; dr <= 1; dr++) {
				for (dc = -1; dc <= 1; dc++) {
					int not_this_cell = (dr != 0) || (dc != 0) || (ds != 0) || (dm != 0);
					int row_valid = ((row + dr) >= 0) && ((row + dr) < rows);
					int col_valid = ((col + dc) >= 0) && ((col + dc) < cols);
					int slice_valid = ((slice + ds) >= 0) && ((slice + ds) < slices);
					int mslice_valid = ((mslice + dm) >= 0) && ((mslice + dm) < mslices);
					if (mslice_valid && slice_valid && row_valid && col_valid && not_this_cell) {
						neighbors += (current[mslice + dm][slice + ds][row + dr][col + dc] == '#');
					}
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
	int mslices = max_steps + 1 + max_steps;
	char (*board)[slices][rows][cols] = calloc(mslices, sizeof(*board));
	char (*next)[slices][rows][cols] = calloc(mslices, sizeof(*next));

	rewind(in);
	int row = 0;
	int col;
	while (fgets(line, 100, in) != NULL) {
		for (col = 0; col < strlen(line); col++) {
			board[max_steps + 0][max_steps + 0][max_steps + row][max_steps + col] = line[col];
		}
		row++;
	}

	int slice;
	int mslice;
	int t;
	for (t = 0; t < max_steps; t++) {
		int population = 0;
		for (mslice = 0; mslice < mslices; mslice++) {
			for (slice = 0; slice < slices; slice++) {
				for (row = 0; row < rows; row++) {
					for (col = 0; col < cols; col++) {
						next[mslice][slice][row][col] = next_gen(mslices, slices, rows, cols, board, mslice, slice, row, col);
						population += (next[mslice][slice][row][col] == '#');
					}
				}
			}
		}
		char (*aux)[slices][rows][cols] = board;
		board = next;
		next = aux;
		printf("Generation %d has population %d.\n", t + 1, population);
	}

	free(next);
	free(board);
	free(line);

	return 0;
}
