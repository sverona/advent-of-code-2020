#include <stdio.h>
#include <stdlib.h>

int main(int argv, char** argc) {
	FILE* in = fopen(argc[1], "r");

	int lines = 0;
	int num;
	while (fscanf(in, "%d", &num) != EOF) {
		lines++;
		printf("%d\n", num);
	}

	return 0;
}
