#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv) {
	int size = 1000;
	int *cups = calloc(size, sizeof(int));
	int *next = calloc(size, sizeof(int));

	// char *initial = "487912365";
	char *initial = "389125467";
	int i;
	for (i = 0; i < strlen(initial); i++) {
		cups[i] = initial[i] - '0';
	}
	for (i = strlen(initial); i < size; i++) {
		cups[i] = i + 1;
	}

	int steps = 100000;
	int step;
	int current = 0;
	for (step = 0; step <= steps; step++) {
		printf("STEP %08d: current is %d\n", step, cups[current]);
		/*
		for (i = 0; i < 10; i++) {
			if (i == current) {
				printf("[%d]", cups[i]);
			} else {
				printf(" %d ", cups[i]);
			}
		}
		printf("\n");
		*/

		int curr_label = cups[current];
		int dest_label = cups[current] - 1;
		if (dest_label < 1) {
			dest_label = 9;
		}
		
		int next1 = cups[(current + 1) % size];
		int next2 = cups[(current + 2) % size];
		int next3 = cups[(current + 3) % size];
		printf("next1: %d next2: %d next3: %d\n", next1, next2, next3);
		while (dest_label == next1 || dest_label == next2 || dest_label == next3) {
			dest_label--;
			if (dest_label < 1) {
				dest_label = 9;
			}
		}

		int copy_idx = 0;
		for (i = 0; i < size; i++){
			int cup = cups[i];
			if (cup == next1 || cup == next2 || cup == next3) {
				continue;
			}
			next[copy_idx++] = cup;
		}

		for (i = 0; i < size; i++) {
			if (next[i] == dest_label) {
				int j;
				for (j = size - 4; j > i; j--) {
					next[j + 3] = next[j];
				}
				next[i + 1] = next1;
				next[i + 2] = next2;
				next[i + 3] = next3;
				break;
			}
		}
		
		for (i = 0; i < size; i++) {
			if (next[i] == curr_label) {
				current = (i + 1) % size;
				break;
			}
		}
		int *aux = cups;
		cups = next;
		next = aux;
	}

	return 0;
}
