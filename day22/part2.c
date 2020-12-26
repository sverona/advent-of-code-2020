#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
	int start;
	int end;
	int size;
	int occupied;
	int player;
	int* q;
} Queue;

int full(Queue* q) {
	return q->occupied >= q->size;
}

int empty(Queue* q) {
	return q->occupied <= 0;
}

void push(Queue* q, int i) {
	if (full(q)) {
		fprintf(stderr, "Cannot push onto full queue\n");
		return;
	}
	q->occupied++;
	q->q[q->end++] = i;
	while (q->end >= q->size) {
		q->end -= q->size;
	}
}

int pop(Queue* q) {
	if (empty(q)) {
		fprintf(stderr, "Cannot pop from empty queue\n");
		exit(1);
	}
	q->occupied--;
	int i = q->q[q->start];
	q->q[q->start++] = 0;
	while (q->start >= q->size) {
		q->start -= q->size;
	}
	return i;
}

int can_recurse(Queue* q, int c) {
	return q->occupied >= c;
}

void deck(Queue* q) {
	int i;
	for (i = 0; i < q->occupied; i++) {
		int c = q->q[(q->start + i) % q->size];
		if (c >= 0) {
			printf("%d ", c);
		}
	}
	printf("\n");
}

int equal(Queue* q1, Queue* q2) {
	int i;
	for (i = 0; i < q1->size; i++) {
		int c1 = q1->q[(q1->start + i) % q1->size];
		int c2 = q2->q[(q2->start + i) % q2->size];
		// printf("new[%d] = %d; old[%d] = %d\n", i, c1, i, c2);
		if (c1 != c2) {
			return 0;
		}
	}
	return 1;
}

int looped(Queue* q1, Queue* q2, Queue** previous1, Queue** previous2, int turns) {
	int turn;
	for (turn = 0; turn < turns; turn++) {
		// printf("Checking current state against turn %d...\n", turn);
		if (equal(q1, previous1[turn]) && equal(q2, previous2[turn])) {
			printf("Found loop\n");
			return 1;
		}
	}
	return 0;
}

int MAX_TURNS = 1000000;
int games = 0;
int depth = 0;
Queue* play(Queue* p1, Queue* p2) {
	int game = games++;
	int turns = 0;
	Queue *winner = p1;
	Queue **previous1 = calloc(MAX_TURNS, sizeof(*previous1));
	Queue **previous2 = calloc(MAX_TURNS, sizeof(*previous2));
	while (!empty(p1) && !empty(p2)) {
		if (turns > MAX_TURNS) {
			fprintf(stderr, "Too many turns\n");
			exit(1);
		}

		previous1[turns] = malloc(sizeof(Queue));
		previous1[turns]->size = p1->size;
		previous1[turns]->start = p1->start;
		previous1[turns]->end = p1->end;
		previous1[turns]->q = calloc(p1->size, sizeof(int));
		int i;
		for (i = 0; i < p1->size; i++) {
			previous1[turns]->q[i] = p1->q[i];
		}

		previous2[turns] = malloc(sizeof(Queue));
		previous2[turns]->size = p2->size;
		previous2[turns]->start = p2->start;
		previous2[turns]->end = p2->end;
		previous2[turns]->q = calloc(p2->size, sizeof(int));
		for (i = 0; i < p2->size; i++) {
			previous2[turns]->q[i] = p2->q[i];
		}

		if (looped(p1, p2, previous1, previous2, turns)) {
			winner = p1;
			break;
		}

		turns++;

		printf("\e[1;1H\e[2J");
		printf("[%d] GAME %d TURN %d\n", depth, game, turns);
		printf("P1: "); deck(p1);
		printf("P2: "); deck(p2);

		int p1_card = pop(p1);
		int p2_card = pop(p2);

		if (can_recurse(p1, p1_card) && can_recurse(p2, p2_card)) {
			Queue *sp1 = malloc(sizeof(Queue));
			sp1->size = p1->size;
			sp1->player = 1;
			sp1->start = 0;
			sp1->end = 0;
			sp1->occupied = 0;
			sp1->q = calloc(p1->size, sizeof(int));
			Queue *sp2 = malloc(sizeof(Queue));
			sp2->size = p2->size;
			sp2->player = 2;
			sp2->start = 0;
			sp2->end = 0;
			sp2->occupied = 0;
			sp2->q = calloc(p2->size, sizeof(int));

			int i, c;
			int p1_copy = p1->start;
			for (i = 0; i < p1_card; i++) {
				c = p1->q[p1_copy++];
				push(sp1, c);
				while (p1_copy >= p1->size) {
					p1_copy -= p1->size;
				}
			}
			int p2_copy = p2->start;
			for (i = 0; i < p2_card; i++) {
				c = p2->q[p2_copy++];
				push(sp2, c);
				while (p2_copy >= p2->size) {
					p2_copy -= p2->size;
				}
			}

			depth++;
			Queue *subwinner = play(sp1, sp2);
			depth--;

			winner = (subwinner->player == 1) ? p1 : p2;

			free(sp1);
			free(sp2);
		} else {
			winner = (p1_card > p2_card) ? p1 : p2;
		}
		printf("%d & %d, p%d wins\n", p1_card, p2_card, winner->player);
		printf("\n");

		push(winner, (winner->player == 1) ? p1_card : p2_card);
		push(winner, (winner->player == 1) ? p2_card : p1_card);
	}

	free(previous1);
	free(previous2);
	return winner;
}

int main(int argc, char** argv) {
	if (argc != 2) {
		fprintf(stderr, "usage: %s input\n", argv[0]);
		exit(1);
	}

	FILE* in = fopen(argv[1], "r");

	char* line = calloc(100, sizeof(char));

	Queue *p1 = malloc(sizeof(Queue));
	p1->player = 1;
	p1->size = 0;
	p1->start = 0;
	p1->end = 0;
	p1->occupied = 0;

	Queue *p2 = malloc(sizeof(Queue));
	p2->player = 2;
	p2->size = 0;
	p2->start = 0;
	p2->end = 0;
	p2->occupied = 0;

	while (fgets(line, 100, in)) {
		if (isdigit(line[0])) {
			p1->size++;
			p2->size++;
		}
	}

	p1->q = calloc(p1->size, sizeof(int));
	p2->q = calloc(p2->size, sizeof(int));

	int player = 0;
	rewind(in);

	while (fgets(line, 100, in)) {
		if (line[0] == 'P') {
			player++;
			continue;
		}
		if (isdigit(line[0])) {
			Queue *p = (player == 1) ? p1 : p2;
			push(p, atoi(line));
		}
	}
	
	int score = 0;
	int i;
	Queue *winner = play(p1, p2);
	for (i = 0; i < winner->size; i++) {
		int c = pop(winner);
		score += c * (winner->size - i);
	}
	
	printf("SCORE %d\n", score);

	free(p1);
	free(p2);
	free(line);

	return 0;
}
