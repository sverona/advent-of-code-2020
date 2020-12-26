#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv) {
	int circle_size = 1000000;

	int *cups = calloc(circle_size + 1, sizeof(int));

	char *initial = "487912365";
	// char *initial = "389125467";
	// initialize circular linked list
	int i;
	int this_cup, next_cup;
	for (i = 0; i < strlen(initial) - 1; i++) {
		this_cup = initial[i] - '0';
		next_cup = initial[(i + 1)] - '0';
		cups[this_cup] = next_cup;
	}
	cups[next_cup] = strlen(initial) + 1;
	for (i = strlen(initial) + 1; i < circle_size; i++) {
		cups[i] = i + 1;
	}
	cups[circle_size] = initial[0] - '0';

	int step;
	int steps = 10000000;
	int current = initial[0] - '0';

	for (step = 0; step <= steps; step++) {
		int next1 = cups[current];
		int next2 = cups[next1];
		int next3 = cups[next2];

		int dest = current;
		do {
			dest--;
			if (dest < 1) {
				dest = circle_size;
			}
		} while (dest == next1 || dest == next2 || dest == next3);

		cups[current] = cups[next3];
		cups[next3] = cups[dest];
		cups[dest] = next1;

		current = cups[current];
	}
	int next1 = cups[1];
	int next2 = cups[next1];
	long product = 1L;
	product *= next1;
	product *= next2;
	printf("%d * %d = %ld\n", next1, next2, product);

	free(cups);

	return 0;
}
