#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
	int inside;
	int container;
	int count;
} Rule;

int bags_inside(int bag_idx, Rule **rules, int rule_count) {
	int total = 0;
	int rule_idx = 0;
	for (rule_idx = 0; rule_idx < rule_count; rule_idx++) {
		Rule *this_rule = rules[rule_idx];
		if (this_rule->container == bag_idx) {
			total += this_rule->count * (1 + bags_inside(this_rule->inside, rules, rule_count));
		}
	}
	return total;
}

int main(int argv, char** argc) {
	FILE* in = fopen(argc[1], "r");

	char* line = calloc(200, sizeof(char));

	int lines = 0;
	while (fgets(line, 200, in) != NULL) {
		lines++;
	}

	char **types = calloc(lines, sizeof(*types));
	int *subtypes = calloc(lines, sizeof(int));

	rewind(in);
	int i = 0;
	while(fgets(line, 200, in) != NULL) {
		types[i] = calloc(30, sizeof(char));

		int first_space = strchr(line, ' ') - line;
		int second_space = strchr(line + first_space + 1, ' ') - line;
		strncpy(types[i], line, second_space);
		i++;
	}

	rewind(in);
	i = 0;
	int rule_count = 0;
	char *token = calloc(20, sizeof(char));
	while (fgets(line, 200, in) != NULL) {
		token = strtok(line, " ");
		while (token != NULL) {
			if (isdigit(token[0])) {
				subtypes[i]++;
				rule_count++;
			}
			token = strtok(NULL, " ");
		}
	};

	Rule **rules = calloc(rule_count, sizeof(Rule));
	rewind(in);
	int type = 0;
	int rule_idx = 0;
	while (fgets(line, 200, in) != NULL) {
		token = strtok(line, " ");
		while (token != NULL) {
			if (isdigit(token[0])) {
				int count = atoi(token);
				char *contained_type = calloc(30, sizeof(char));

				strcpy(contained_type, strtok(NULL, " "));
				contained_type[strlen(contained_type)] = ' ';
				strcpy(contained_type + strlen(contained_type), strtok(NULL, " "));

				int contained_idx;
				for (contained_idx = 0; contained_idx < lines; contained_idx++) {
					if (strcmp(types[contained_idx], contained_type) == 0) {
						break;
					}
				}

				Rule *this_rule = malloc(sizeof(Rule));
				this_rule->inside = contained_idx;
				this_rule->container = type;
				this_rule->count = count;

				rules[rule_idx] = this_rule;

				subtypes[type]++;
				rule_idx++;
			}
			token = strtok(NULL, " ");
		}
		type++;
	};

	int shiny;
	for (shiny = 0; shiny < lines; shiny++) {
		if (strcmp(types[shiny], "shiny gold") == 0) {
			break;
		}
	}
	printf("%d\n", shiny);

	printf("%d\n", bags_inside(shiny, rules, rule_count));


	free(types);
	free(line);
	free(rules);

	return 0;
}
