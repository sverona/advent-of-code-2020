#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int main(int argc, char** argv) {
	if (argc != 2) {
		fprintf(stderr, "usage: %s input\n", argv[0]);
		exit(1);
	}

	FILE* in = fopen(argv[1], "r");

	int line_size = 100;
	int floor_size = 2 * line_size + 1;
	int (*floor)[floor_size] = calloc(floor_size, sizeof(*floor));
	char* line = calloc(line_size, sizeof(char));
	char* strip = calloc(line_size, sizeof(char));
	int alpha, beta;
	while (fgets(line, line_size, in)) {
		strip = strtok(line, "\n");
		hex(strip, &alpha, &beta);
		printf("%s %d %d\n", strip, alpha, beta);
		floor[line_size + alpha][line_size + beta]++;
	}

	int black = 0;
	for (alpha = 0; alpha < floor_size; alpha++) {
		for (beta = 0; beta < floor_size; beta++) {
			if (floor[alpha][beta] % 2) {
				black++;
			}
		}
	}

	printf("BLACK %d\n", black);

	free(floor);
	free(line);

	return 0;
}
