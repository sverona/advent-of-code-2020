#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct {
	int id;
	char **data;
	int xflip;
	int rotation;
} Piece;

void affine_transform(int rotation, int xflip, int array_size, int* x, int* y) {
	int sin[4] = {0, 1, 0, -1};
	int cos[4] = {1, 0, -1, 0};

	float x0 = (array_size - 1) / 2.0;
	float y0 = x0;

	int f = xflip ? -1 : 1;

	float xf = (*x - x0) * f * cos[rotation] - (*y - y0) * sin[rotation] + x0;
	float yf = (*x - x0) * f * sin[rotation] + (*y - y0) * cos[rotation] + y0;

	// printf("%d %d -> %f %f\n", *x, *y, xf, yf);

	*x = (int)round(xf);
	*y = (int)round(yf);
}

void border(int d, int size, int* start, int* di) {
	if (d == 0) {
		*start = 1;
		*di = 1;
	} else if (d == -1) {
		*start = 0;
		*di = 0;
	} else if (d == 1) {
		*start = size - 1;
		*di = 0;
	}
}

int check_border(int width, int height, int row, int col, int my_nbr_dr, int my_nbr_dc, Piece* soln[width][height]) {
	if (!my_nbr_dr && !my_nbr_dc) {
		return 1;
	}

	int nbr_row = row + my_nbr_dr;
	int nbr_col = col + my_nbr_dc;

	int nbr_row_valid = (nbr_row >= 0) && (nbr_row < height);
	int nbr_col_valid = (nbr_col >= 0) && (nbr_col < width);

	if (!nbr_row_valid || !nbr_col_valid) {
		return 1;
	}

	Piece *me = soln[row][col];
	Piece *nbr = soln[nbr_row][nbr_col];

	if (!me || !nbr) {
		return 1;
	}

	int size = strlen(me->data[0]);
	int my_start_row, my_range_dr;
	border(my_nbr_dr, size, &my_start_row, &my_range_dr);
	int my_start_col, my_range_dc;
	border(my_nbr_dc, size, &my_start_col, &my_range_dc);
	int nbr_start_row, nbr_range_dr;
	border(-my_nbr_dr, size, &nbr_start_row, &nbr_range_dr);
	int nbr_start_col, nbr_range_dc;
	border(-my_nbr_dc, size, &nbr_start_col, &nbr_range_dc);

	int i;
	for (i = 0; i < size - 2; i++) {
		int my_row = my_start_row + i * my_range_dr;
		int my_col = my_start_col + i * my_range_dc;
		int nbr_row = nbr_start_row + i * nbr_range_dr;
		int nbr_col = nbr_start_col + i * nbr_range_dc;
		affine_transform(me->rotation, me->xflip, size, &my_row, &my_col);
		affine_transform(nbr->rotation, nbr->xflip, size, &nbr_row, &nbr_col);

		char mine = me->data[my_row][my_col];
		char nbrs = nbr->data[nbr_row][nbr_col];

		if (mine != nbrs) {
			return 0;
		}
	}
	return 1;
}

int is_valid(int width, int height, int row, int col, Piece* soln[width][height]) {
	// Check the borders of [row, col.]
	int dr, dc;
	for (dr = -1; dr <= 1; dr++) {
		for (dc = -1; dc <= 1; dc++) {
			if (!check_border(width, height, row, col, dr, dc, soln)) {
				return 0;
			}
		}
	}
	return 1;
}

int is_complete(int width, int height, Piece* soln[width][height]) {
	int row, col;
	for (row = 0; row < height; row++) {
		for (col = 0; col < width; col++) {
			if (!soln[row][col]) {
				return 0;
			}
		}
	}
	return 1;
}

void antidiagonal(int size, int pos, int* row, int* col) {
	int invert = (pos >= size * (size + 1) / 2);
	if (invert) {
		pos = size * size - 1 - pos;
	}

	int arow;
	int top_of_row = 0;
	for (arow = 0; pos >= top_of_row; arow++) {
		top_of_row = arow * (arow + 1) / 2;
	}

	int top_of_last_row = top_of_row - arow;
	int acol = pos - top_of_last_row;

	if (invert) {
		*row = (size - (arow - acol));
		*col = (size - acol);
	} else {
		*row = arow - acol - 1;
		*col = acol - 1;
	}
}

int backtrack_recurse(int width, int height, int pos, Piece** pieces, Piece* soln[width][height]) {
	int row, col;
	antidiagonal(width, pos, &row, &col);

	int prevrow, prevcol;
	antidiagonal(width, pos - 1, &prevrow, &prevcol);

	if (!is_valid(width, height, prevrow, prevcol, soln)) {
		return 0;
	}

	/*
	if (pos > 0) {
		printf("\e[1;1H\e[2J");
		int r, c;
		for (r = 0; r < height; r++) {
			for (c = 0; c < width; c++) {
				if (soln[r][c]) {
					printf("%d ", soln[r][c]->id);
				} else {
					printf("     ");
				}
			}
			printf("\n");
		}
	}
	*/

	if (is_complete(width, height, soln)) {
		return 1;
	} else {
		// can't be too safe...
		if (pos == width * height) {
			return 0;
		}

		int i, piece_idx;
		int j;

		for (piece_idx = 0; piece_idx < width * height; piece_idx++) {
			int already_placed = 0;
			for (j = 0; j < pos; j++) {
				int jrow, jcol;
				antidiagonal(width, j, &jrow, &jcol);
				Piece *jpiece = soln[jrow][jcol];
				if (jpiece && jpiece->id == pieces[piece_idx]->id) {
					already_placed = 1;
					break;
				}
			}
			if (already_placed) {
				continue;
			}
			soln[row][col] = pieces[piece_idx];
			for (i = 0; i < 8; i++) {
				soln[row][col]->xflip = i / 4;
				soln[row][col]->rotation = i % 4;

				if (backtrack_recurse(width, height, pos + 1, pieces, soln)) {
					return 1;
				}
			}
		}
		soln[row][col] = NULL;
		return 0;
	}
}

int backtrack(int width, int height, Piece** pieces, Piece* soln[width][height]) {
	return backtrack_recurse(width, height, 0, pieces, soln);
}

int search(int image_size, char image[image_size][image_size], int row, int col, int rotation, int xflip) {
	int monster[15][2] = {{0, 18},
			      {1, 0},
			      {1, 5},
			      {1, 6},
			      {1, 11},
			      {1, 12},
			      {1, 17},
			      {1, 18},
			      {1, 19},
			      {2, 1},
			      {2, 4},
			      {2, 7},
			      {2, 10},
			      {2, 13},
			      {2, 16}};

	int i;
	for (i = 0; i < 15; i++) {
		int dr = monster[i][0];
		int dc = monster[i][1];
		int checkr = row + dr, checkc = col + dc;
		int row_good = checkr >= 0 && checkr < image_size;
		int col_good = checkc >= 0 && checkc < image_size;

		if (!row_good || !col_good) {
			return 0;
		}

		affine_transform(rotation, xflip, image_size, &checkr, &checkc);
		if (image[checkr][checkc] == '.') {
			return 0;
		}
	}
	return 1;
}

int main(int argc, char** argv) {
	if (argc != 2) {
		fprintf(stderr, "usage: %s input\n", argv[0]);
		exit(1);
	}

	FILE* in = fopen(argv[1], "r");

	char* line = calloc(100, sizeof(char));
	int tile_count = 0;
	int tile_size = 0;
	while (fgets(line, 100, in) != NULL) {
		if (line[0] == '\n') {
			continue;
		}
		if (line[0] == 'T') {
			tile_count++;
			tile_size = 0;
		} else {
			tile_size++;
		}
	}

	int grid_size = 0;
	while (grid_size * grid_size < tile_count) {
		grid_size++;
	}

	Piece **pieces = calloc(tile_count, sizeof(*pieces));

	rewind(in);
	int tile_no = -1;
	int tile_row = 0;
	while (fgets(line, 100, in) != NULL) {
		if (line[0] == '\n') {
			continue;
		} else if (line[0] == 'T') {
			int id;
			int success = sscanf(line, "Tile %d:", &id);
			if (!success) {
				fprintf(stderr, "Couldn't parse %s", line);
				exit(1);
			}
			char **data = calloc(tile_size, sizeof(*data));
			tile_no++;

			pieces[tile_no] = malloc(sizeof(Piece));
			pieces[tile_no]->data = data;
			pieces[tile_no]->id = id;
			tile_row = 0;
		} else {
			pieces[tile_no]->data[tile_row] = calloc(tile_size, sizeof(char));
			strncpy(pieces[tile_no]->data[tile_row], line, tile_size);
			tile_row++;
		}
	}

	Piece* (*soln)[grid_size] = calloc(grid_size, sizeof(*soln));

	int good = backtrack(grid_size, grid_size, pieces, soln);
	if (good) {
		int image_size = grid_size * (tile_size - 2);
		char (*image)[image_size] = calloc(image_size, sizeof(*image));

		int row, col;
		for (row = 0; row < image_size; row++) {
			for (col = 0; col < image_size; col++) {
				int big_row = row / (tile_size - 2);
				int big_col = col / (tile_size - 2);
				int small_row = row % (tile_size - 2) + 1;
				int small_col = col % (tile_size - 2) + 1;

				Piece* piece = soln[big_row][big_col];

				affine_transform(piece->rotation, piece->xflip, tile_size, &small_row, &small_col);
				image[row][col] = piece->data[small_row][small_col];
				printf("%c", image[row][col]);
			}
			printf("\n");
		}
		int monsters = 0;
		
		int rotation, xflip;
		for (xflip = 0; xflip <= 1; xflip++) {
			for (rotation = 0; rotation < 4; rotation++) {
				for (row = 0; row < image_size; row++) {
					for (col = 0; col < image_size; col++) {
						if (search(image_size, image, row, col, rotation, xflip)) {
							monsters++;
							printf("Monster %d located; rotation %d xflip %d row %d col %d\n",
								monsters, rotation, xflip, row, col);
						}
					}
				}
			}
		}

		int hashes = 0;
		for (row = 0; row < image_size; row++) {
			for (col = 0; col < image_size; col++) {
				if (image[row][col] == '#') {
					hashes++;
				}
			}
		}

		printf("HASHES %d\n", hashes);
		printf("NON-MONSTER HASHES %d\n", hashes - monsters * 15);

		free(image);
	}

	free(soln);
	free(pieces);
	free(line);

	return 0;
}
