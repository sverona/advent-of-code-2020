#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argv, char** argc) {
	if (argv != 2) {
		fprintf(stderr, "usage: %s [input]", argc[0]);
		exit(1);
	}

	FILE* in = fopen(argc[1], "r");

	int valid_count = 0;
	int min, max;
	char ch;
	char* pass = calloc(100, sizeof(char));
	while (fscanf(in, "%d-%d %c: %s", &min, &max, &ch, pass) != EOF) {
		printf("%d, %d, %c, %s\n", min, max, ch, pass);

		int first_good = (pass[min - 1] == ch);
		int second_good = (pass[max - 1] == ch);

		if (first_good != second_good) {
			printf("VALID %d-%d %c %s\n", min, max, ch, pass);
			valid_count++;
		}
	}
	printf("VALID TOTAL %d\n", valid_count);

	return 0;
}
