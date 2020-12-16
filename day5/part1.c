#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int binsearch(char* code, char left, char right) {
	int lower = 0;
	int upper = (1 << strlen(code)) - 1;

	int i;
	for (i = 0; i < strlen(code) - 1; i++) {
		int newlower = 0, newupper = 0;
		int mid = (lower + upper) / 2;
		if (code[i] == left) {
			newlower = lower;
			newupper = mid;
		} else if (code[i] == right) {
			newlower = mid + 1;
			newupper = upper;
		} else {
			fprintf(stderr, "bad character %c\n", code[i]);
			exit(1);
		}
		lower = newlower;
		upper = newupper;
		printf("%c %d %d\n", code[i], lower, upper);
	}

	char lastchar = code[strlen(code) - 1];
	printf("%c %d %d\n", lastchar, lower, upper);
	if (lastchar == left) {
		return lower;
	} else if (lastchar == right) {
		return upper;
	} else {
		fprintf(stderr, "bad character %c\n", lastchar);
		exit(1);
	}
}

int main(int argv, char** argc) {
	FILE* in = fopen(argc[1], "r");

	char* line = calloc(100, sizeof(char));
	char* rowcode = calloc(10, sizeof(char));
	char* seatcode = calloc(5, sizeof(char));

	int maxid = 0;

	while (fgets(line, 100, in) != NULL) {
		strncpy(rowcode, line, 7);
		strncpy(seatcode, line + 7, 3);

		int row = binsearch(rowcode, 'F', 'B');
		int seat = binsearch(seatcode, 'L', 'R');
		int id = 8 * row + seat;
		if (id > maxid) {
			maxid = id;
		}
		printf("%s %s %d %d %d\n", rowcode, seatcode, row, seat, row * 8 + seat);
	}

	free(seatcode);
	free(rowcode);
	free(line);

	printf("MAXID %d\n", maxid);

	return 0;
}
