#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void hex(char* ref, int* alpha, int* beta) {
	int i = 0;
	int a = 0;
	int b = 0;
	while (i < strlen(ref)) {
		switch(ref[i++]) {
		case 'e':
			a++;
			break;
		case 'w':
			a--;
			break;
		case 'n':
			switch(ref[i++]) {
			case 'e':
				a++;
				b++;
				break;
			case 'w':
				b++;
				break;
			default:
				fprintf(stderr, "Bad character %c", ref[i]);
				exit(1);
			}
			break;
		case 's':
			switch(ref[i++]) {
			case 'e':
				b--;
				break;
			case 'w':
				a--;
				b--;
				break;
			default:
				fprintf(stderr, "Bad character %c", ref[i]);
				exit(1);
			}
			break;
		default:
			fprintf(stderr, "Bad character %c", ref[i - 1]);
			exit(1);
		}
	}
	*alpha = a;
	*beta = b;
}

int next_gen(int floor_size, int floor[][floor_size], int alpha, int beta) {
	int neighbors = 0;
	int da[6] = {1, 0, -1, -1, 0, 1};
	int db[6] = {0, -1, -1, 0, 1, 1};
	int i;
	for (i = 0; i < 6; i++) {
		int a_good = (alpha + da[i]) >= 0 && (alpha + da[i]) < floor_size;
		int b_good = (beta + db[i]) >= 0 && (beta + db[i]) < floor_size;
		if (a_good && b_good) {
			int nbr = floor[alpha + da[i]][beta + db[i]];
			if (nbr) {
				neighbors++;
			}
		}
	}

	if (floor[alpha][beta]) {
		return (neighbors == 1) || (neighbors == 2);
	} else {
		return (neighbors == 2);
	}
}

int main(int argc, char** argv) {
	if (argc != 2) {
		fprintf(stderr, "usage: %s input\n", argv[0]);
		exit(1);
	}

	FILE* in = fopen(argv[1], "r");

	int line_size = 100;
	int steps = 100;
	int floor_size = 2 * (line_size + steps) + 1;
	int (*floor)[floor_size] = calloc(floor_size, sizeof(*floor));
	int (*next)[floor_size] = calloc(floor_size, sizeof(*floor));
	char* line = calloc(line_size, sizeof(char));
	char* strip = calloc(line_size, sizeof(char));
	int alpha, beta;
	while (fgets(line, line_size, in)) {
		strip = strtok(line, "\n");
		hex(strip, &alpha, &beta);
		// printf("%s %d %d\n", strip, alpha, beta);
		floor[steps + line_size + alpha][steps + line_size + beta] ^= 1;
	}

	int step;
	for (step = 0; step <= steps; step++) {
		for (alpha = 0; alpha < floor_size; alpha++) {
			for (beta = 0; beta < floor_size; beta++) {
				next[alpha][beta] = next_gen(floor_size, floor, alpha, beta);
			}
		}
		int black = 0;
		for (alpha = 0; alpha < floor_size; alpha++) {
			for (beta = 0; beta < floor_size; beta++) {
				if (floor[alpha][beta]) {
					black++;
				}
			}
		}

		printf("DAY %d BLACK %d\n", step, black);
		int (*aux)[floor_size] = floor;
		floor = next;
		next = aux;

	}

	free(next);
	free(floor);
	free(line);

	return 0;
}
