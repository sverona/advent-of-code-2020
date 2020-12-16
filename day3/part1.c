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

	int ypos = 0;
	int slope = 3;
	int trees = 0;
	while (fscanf(in, "%s", row) != EOF) {
		int xpos = (slope * ypos) % columns;
		if (row[xpos] == '#') {
			trees++;
		}
		ypos += 1;
	}
	printf("%d\n", trees);

	return 0;
}
