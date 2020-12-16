#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argv, char** argc) {
	FILE* in = fopen(argc[1], "r");

	int lines = 0;
	char *line = calloc(200, sizeof(char));

	char *fields[8] = {"byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid", "cid"};
  char *colors[7] = {"amb", "blu", "brn", "gry", "grn", "hzl", "oth"};

	int valids[8] = {0, 0, 0, 0, 0, 0, 0, 0};

	int valid_count = 0;

	char *token = calloc(20, sizeof(char));
	int j;
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
			token = strtok(line, " \r");
			while (token != NULL) {
				int i;
				for (i = 0; i < 8; i++){
					if (strstr(token, fields[i]) == token) {
						char *value = calloc(20, sizeof(char));
						strcpy(value, token + 4);
						printf("%s %s %d\n", fields[i], value, strlen(value));
						switch (i) {
						case 1: // byr
								if (strlen(value) == 4 &&
												isdigit(value[0]) &&
												isdigit(value[1]) &&
												isdigit(value[2]) &&
												isdigit(value[3])) {
										int yr = atoi(value);
										if (yr >= 1920 && yr <= 2002) {
												valids[i] = 1;
										} else {
												valids[i] = 0;
										}

								} else {
										valids[i] = 0;
								}
								break;
						case 2: // iyr
								if (strlen(value) == 4 &&
												isdigit(value[0]) &&
												isdigit(value[1]) &&
												isdigit(value[2]) &&
												isdigit(value[3])) {
										int yr = atoi(value);
										if (yr >= 2010 && yr <= 2020) {
												valids[i] = 1;
										} else {
												valids[i] = 0;
										}
								}
								break;
						case 3: // iyr
								if (strlen(value) == 4 &&
												isdigit(value[0]) &&
												isdigit(value[1]) &&
												isdigit(value[2]) &&
												isdigit(value[3])) {
										int yr = atoi(value);
										if (yr >= 2020 && yr <= 2030) {
												valids[i] = 1;
										} else {
												valids[i] = 0;
										}

								} else {
										valids[i] = 0;
								}
								break;
						case 4: // hgt
								if (strstr(value, "cm") != NULL) {
										char *len = calloc(20, sizeof(char));
										strncpy(len, value, strlen(value) - 2);

										int ln = atoi(len);
										printf("hgt- %d\n", ln);
										if (ln >= 150 && ln <= 193) {
												valids[i] = 1;
										} else {
												valids[i] = 0;
										}
								} else if (strstr(value, "in") != NULL) {
										char *len = calloc(20, sizeof(char));
										strncpy(len, value, strlen(value) - 2);

										int ln = atoi(len);
										printf("hgt- %d\n", ln);
										if (ln >= 59 && ln <= 76) {
												valids[i] = 1;
										} else {
												valids[i] = 0;
										}
								}
								break;
						case 5: // hcl
								if (strlen(value) == 7 && value[0] == '#') {
										valids[i] = 1;
										for (j = 1; j < 7; j++) {
												if (isxdigit(value[i]) && (isdigit(value[i]) || islower(value[i]))) {
												} else {
														valids[i] = 0;
												}
										}
								}
								break;
						case 6:; // ecl
								for (j = 0; j < 7; j++) {
										if (strcmp(colors[j], value) == 0) {
										    printf("%d\n",j);
												valids[i] = 1;
										}
								}
						case 7: // pid
								if (strlen(value) == 9) {
										valids[i] = 1;
										for (j = 0; j < 9; j++) {
												if (! isdigit(value[i])) {
														valids[i] = 0;
												}
										}
								}
					}
				}
			}
      token = strtok(NULL, " ");
		}
	}
	}

	printf("VALID COUNT %d\n", valid_count);

	return 0;
}
