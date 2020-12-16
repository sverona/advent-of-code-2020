#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
	int inside;
	int container;
	int count;
} Rule;

typedef struct Node {
	struct Node *next;
	void *value;
} Node;

int main(int argv, char** argc) {
	FILE* in = fopen(argc[1], "r");

	char* line = calloc(200, sizeof(char));

	int lines = 0;
	while (fgets(line, 200, in) != NULL) {
		lines++;
		printf("%s", line);
	}

	printf("%d\n", lines);
	char **types = calloc(lines, sizeof(*types));

	rewind(in);
	int i = 0;
	while(fgets(line, 200, in) != NULL) {
		types[i] = calloc(30, sizeof(char));

		int first_space = strchr(line, ' ') - line;
		int second_space = strchr(line + first_space + 1, ' ') - line;
		strncpy(types[i], line, second_space);
		printf("%d %s\n", i, types[i]);
		i++;
	}

	rewind(in);
	Node *rule_list = malloc(sizeof(Node));
	Node *this_node = rule_list;
	i = 0;
	int rule_count = 0;
	char *token = calloc(20, sizeof(char));
	while (fgets(line, 200, in) != NULL) {
		token = strtok(line, " ");
		while (token != NULL) {
			if (isdigit(token[0])) {
				rule_count++;
				int count = atoi(token);
				char *contained_type = calloc(30, sizeof(char));

				strcpy(contained_type, strtok(NULL, " "));
				contained_type[strlen(contained_type)] = ' ';
				strcpy(contained_type + strlen(contained_type), strtok(NULL, " "));
				// printf("%s\n", contained_type);

				int contained_idx;
				for (contained_idx = 0; contained_idx < lines; contained_idx++) {
					if (strcmp(types[contained_idx], contained_type) == 0) {
						break;
					}
				}

				Rule *this_rule = malloc(sizeof(Rule));
				this_rule->inside = contained_idx;
				this_rule->container = i;
				this_rule->count = count;
				this_node->value = this_rule;

				this_node->next = malloc(sizeof(Node));
				this_node = this_node->next;
				this_node->next = NULL;
			}
			token = strtok(NULL, " ");
		}
		i++;
	}

	for (i = 0; i < lines; i++) {
		if(strcmp(types[i], "shiny gold") == 0) {
			break;
		}
	}
	printf("%d\n", i);
	
	int *stack = calloc(rule_count, sizeof(int));
	int *seen = calloc(lines, sizeof(int));

	int top = 0;
	stack[top] = i;
	int valid_types = 0;

	while (top >= 0) {
		int this_bag_type = stack[top];
		if (strcmp(types[this_bag_type], "shiny gold") != 0) {
			valid_types++;
		}
		printf("Popping %s\n", types[this_bag_type]);
		top -= 1;
		Node *iter = rule_list;
		for (i = 0; i < rule_count; i++) {
			Rule *this_rule = iter->value;
			int this_idx = this_rule->inside;
			if (this_idx == this_bag_type) {
				int container_idx = this_rule->container;
				if (!seen[container_idx]) {
					seen[container_idx] = 1;
					top++;
					printf("Pushing %s\n", types[container_idx]);
					stack[top] = container_idx;
				}
			}
			iter = iter->next;
		}
	}

	free(types);
	free(line);
	free(stack);

	printf("VALID TYPES %d\n", valid_types);

	return 0;
}
