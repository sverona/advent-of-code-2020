#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argv, char** argc) {
	FILE* in = fopen(argc[1], "r");

	int lines = 0;
	int columns = 0;
	char* row = calloc(100, sizeof(char));
	while (fscanf(in, "%s", row) != EOF) {
		lines++;
		columns = strlen(row);
	}

	rewind(in);

	char* array = calloc(lines * columns, sizeof(char));
	int line = 0;
	while(fscanf(in, "%s", row) != EOF) {
		strncpy(&array[line * columns], row, columns);
		line++;
	}

	int directions[5][2] = {{1, 1}, {3, 1}, {5, 1}, {7, 1}, {1, 2}};

	int product = 1;
	int i;
	for (i = 0; i < 5; i++) {
		int x = directions[i][0], y = directions[i][1];
		int this_count = 0;
		int xpos = 0, ypos = 0;
		for (xpos = 0, ypos = 0; ypos < lines; xpos = (xpos + x) % columns, ypos += y) {
			if (array[ypos * columns + xpos] == '#') {
				this_count++;
			}
		}

		printf("%d %d %d\n", x, y, this_count);
		product *= this_count;
	}
	printf("%d\n", product);

	free(row);
	free(array);

	return 0;
}
