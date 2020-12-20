#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

void infix_to_postfix(char* infix, char* postfix) {
	char* opstack = calloc(strlen(infix), sizeof(char));

	int i;
	int op_top = 0;
	int out_top = 0;
	char op;
	for (i = 0; i < strlen(infix); i++) {
		char token = infix[i];
		if (token != ' ') {
			// printf("%d %c %s %s\n", i, token, opstack, postfix);
		}
		
		if (token == '+' || token == '(') {
			// always push
			opstack[op_top++] = token;
		} else if (token == '*') {
			if (op_top == 0 || opstack[op_top - 1] == '(') {
				// push
				opstack[op_top++] = token;
			} else {
				while (op_top > 0 && opstack[op_top - 1] == '+') {
					// pop
					op = opstack[op_top - 1];
					opstack[--op_top] = 0;

					postfix[out_top++] = op;
				}
				// push
				opstack[op_top++] = token;
			}
		} else if (token == ')') {
			while (op_top > 0 && opstack[op_top - 1] != '(') {
				// pop
				op = opstack[op_top - 1];
				opstack[--op_top] = 0;

				postfix[out_top++] = op;
			}
			// also pop the left paren, if there is one
			if (op_top > 0) {
				opstack[--op_top] = 0;
			}
		} else if (isdigit(token)) {
			postfix[out_top++] = token;
		}
	}

	while (op_top > 0) {
		postfix[out_top++] = opstack[--op_top];
	}

	free(opstack);
}

long long eval(char* postfix) {
	long long *evalstack = calloc(strlen(postfix), sizeof(long long));
	int eval_top = 0;

	int i;
	for (i = 0; i < strlen(postfix); i++) {
		char token = postfix[i];
		if (isdigit(token)) {
			evalstack[eval_top++] = (long long)(token - '0');
		} else if (token == '+') {
			long long left = evalstack[--eval_top];
			long long right = evalstack[--eval_top];
			evalstack[eval_top++] = left + right;
		} else if (token == '*') {
			long long left = evalstack[--eval_top];
			long long right = evalstack[--eval_top];
			evalstack[eval_top++] = left * right;
		}
	}
	
	long long result = evalstack[0];
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
	long long total = 0LL;
	while (fgets(line, LINE_LENGTH, in) != NULL) {
		memset(postfix, 0, LINE_LENGTH);
		infix_to_postfix(line, postfix);
		printf("%s%s\n", line, postfix);
		long long result = eval(postfix);
		printf("%lld\n", result);
		total += result;
	}

	printf("TOTAL %lld\n", total);

	free(postfix);
	free(line);

	return 0;
}
