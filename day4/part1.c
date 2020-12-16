#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argv, char** argc) {
	FILE* in = fopen(argc[1], "r");

	int lines = 0;
	char *line = calloc(200, sizeof(char));

	char *fields[8] = {"byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid", "cid"};

	int valids[8] = {0, 0, 0, 0, 0, 0, 0, 0};

	int valid_count = 0;

	char *token = calloc(20, sizeof(char));
	while (fgets(line, 200, in) != NULL) {
		//printf("%d %s\n", strlen(line), line);
		if (line[0] == '\n') {
			// validate
			int i;
			for (i = 0; i < sizeof(valids)/sizeof(int); i++) {
				printf("%d ", valids[i]);
			}
			printf("\n");

			int valid = 1;
			for (i = 0; i < sizeof(valids)/sizeof(int); i++) {
				if (valids[i] == 0 && i != 7) {
					valid = 0;
				}
				valids[i] = 0;
			}
			
			if (valid == 1) {
				valid_count++;
			}
		} else {
			token = strtok(line, " ");
			while (token != NULL) {
				int i;
				for (i = 0; i < 8; i++){
					if (strstr(token, fields[i]) == token) {
						printf("%s %s\n", fields[i], token);
						valids[i] = 1;
					}
				}
				token = strtok(NULL, " ");
			}
		}
	}

	printf("VALID COUNT %d\n", valid_count);

	return 0;
}
