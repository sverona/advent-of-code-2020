#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct {
	int start;
	int end;
	int size;
	int occupied;
	int* q;
} Queue;

int full(Queue* q) {
	return q->occupied >= q->size;
}

int empty(Queue* q) {
	return q->occupied <= 0;
}

int top(Queue* q) {
	return q->q[q->start];
}

void push(Queue* q, int i) {
	if (full(q)) {
		fprintf(stderr, "Cannot push onto full queue\n");
		return;
	}
	if (q->q[q->end] == NULL) {
		q->occupied++;
		q->q[q->end++] = i;
		while (q->end >= q->size) {
			q->end -= q->size;
		}
	}
}

int pop(Queue* q) {
	if (empty(q)) {
		fprintf(stderr, "Cannot pop from empty queue\n");
		exit(1);
	}
	q->occupied--;
	int i = q->q[q->start];
	q->q[q->start++] = NULL;
	while (q->start >= q->size) {
		q->start -= q->size;
	}
	return i;
}

Queue* play(Queue* p1, Queue* p2) {
	int turns = 0;
	Queue *winner = p1;
	while (!empty(p1) && !empty(p2)) {
		int p1_card = pop(p1);
		int p2_card = pop(p2);

		winner = (p1_card > p2_card) ? p1 : p2;
		printf("Turn %d: %d & %d, p%d wins\n", ++turns, p1_card, p2_card, (p1_card > p2_card)? 1 : 2);

		push(winner, (p1_card > p2_card) ? p1_card : p2_card);
		push(winner, (p1_card > p2_card) ? p2_card : p1_card);
	}

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
	p1->size = 0;
	p1->start = 0;
	p1->end = 0;
	p1->occupied = 0;

	Queue *p2 = malloc(sizeof(Queue));
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
