#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

void infix_to_postfix(char* infix, char* postfix) {
	char* opstack = calloc(strlen(infix), sizeof(char));

	int i;
	int op_top = 0;
	int out_top = 0;
	for (i = 0; i < strlen(infix); i++) {
		char token = infix[i];
		// printf("%d %c %s %s\n", i, token, opstack, postfix);

		if (token == '+' || token == '*' || token == '(') {
			opstack[op_top++] = token;
		} else if (token == ')') {
			char op;
			while ((op = opstack[--op_top]) != '(') {
				opstack[op_top + 1] = 0;
				postfix[out_top++] = op;
			}
			if (op_top > 0 && opstack[op_top - 1] != '(') {
				opstack[op_top] = 0;
				postfix[out_top++] = opstack[--op_top];
			}
		} else if (isdigit(token)) {
			postfix[out_top++] = token;
			/*
			if (op_top > 0 && opstack[op_top - 1] != '(') {
				opstack[op_top] = 0;
				postfix[out_top++] = opstack[--op_top];
			}
			*/
		}
	}

	for (i = 0; i < op_top; i++) {
		postfix[out_top++] = opstack[i];
	}

	free(opstack);
}

long eval(char* postfix) {
	long *evalstack = calloc(strlen(postfix), sizeof(long));
	int eval_top = 0;

	int i;
	for (i = 0; i < strlen(postfix); i++) {
		char token = postfix[i];
		if (isdigit(token)) {
			evalstack[eval_top++] = token - '0';
		} else if (token == '+') {
			long left = evalstack[--eval_top];
			long right = evalstack[--eval_top];
			evalstack[eval_top++] = left + right;
		} else if (token == '*') {
			long left = evalstack[--eval_top];
			long right = evalstack[--eval_top];
			evalstack[eval_top++] = left * right;
		}
	}
	
	long result = evalstack[0];
	free(evalstack);

	return result;
}


int main(int argc, char** argv) {
	if (argc != 2) {
		fprintf(stderr, "usage: %s input\n", argv[0]);
		exit(1);
	}

	FILE* in = fopen(argv[1], "r");

	size_t LINE_LENGTH = 200;
	char* line = calloc(LINE_LENGTH, sizeof(char));
	char* postfix = calloc(LINE_LENGTH, sizeof(char));
	long total = 0L;
	while (fgets(line, LINE_LENGTH, in) != NULL) {
		memset(postfix, 0, LINE_LENGTH);
		infix_to_postfix(line, postfix);
		printf("%s%s\n", line, postfix);
		long result = eval(postfix);
		printf("%ld\n", result);
		total += result;
	}

	printf("TOTAL %ld\n", total);

	free(postfix);
	free(line);

	return 0;
}
