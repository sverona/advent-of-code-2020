#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int matches(char* pattern, char* string) {
	if (strlen(pattern) != strlen(string)) {
		return 0;
	}

	int i;
	for (i = 0; i < strlen(pattern); i++) {
		char type = pattern[i];
		int this_char_good = 1;
		switch (type) {
		case '.':
			this_char_good = isalnum(string[i]);
			break;
		case '@':
			this_char_good = isalpha(string[i]);
			break;
		case '0':
			this_char_good = isdigit(string[i]);
			break;
		case 'x':
			this_char_good = isxdigit(string[i]) && (! isupper(string[i]));
			break;
		default:
			// Dumb match
			this_char_good = (string[i] == type);
		}

		if (!this_char_good) {
			return 0;
		}
	}
	return 1;
}

int main(int argv, char** argc) {
	FILE* in = fopen(argc[1], "r");

	char *line = calloc(200, sizeof(char));

	char *fields[8] = {"byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid", "cid"};
	char *colors[7] = {"amb", "blu", "brn", "gry", "grn", "hzl", "oth"};

	int valids[8] = {0, 0, 0, 0, 0, 0, 0, 0};

	int valid_count = 0;

	char *token = calloc(20, sizeof(char));
	while (fgets(line, 200, in) != NULL) {
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
			token = strtok(line, " \n");
			while (token != NULL) {
				char *field = calloc(5, sizeof(char));
				strncpy(field, token, 3);
				char *value = calloc(15, sizeof(char));
				strncpy(value, token + 4, 14);

				int field_idx;
				for (field_idx = 0; field_idx < sizeof(fields)/sizeof(fields[0]); field_idx++) {
					if (strstr(field, fields[field_idx]) == field) {
						break;
					}
				}


				int this_field_valid = 0;
				switch (field_idx) {
				case 0: // byr
					if (matches("0000", value)) {
						int yr = atoi(value);
						this_field_valid = (1920 <= yr) && (yr <= 2002);
					} else {
						this_field_valid = 0;
					}
					break;
				case 1: // iyr
					if (matches("0000", value)) {
						int yr = atoi(value);
						this_field_valid = (2010 <= yr) && (yr <= 2020);
					} else {
						this_field_valid = 0;
					}
					break;
				case 2: // eyr
					if (matches("0000", value)) {
						int yr = atoi(value);
						this_field_valid = (2020 <= yr) && (yr <= 2030);
					} else {
						this_field_valid = 0;
					}
					break;
				case 3: // hgt
					if (matches("000cm", value)) {
						char* hgtstr = calloc(5, sizeof(char));
						strncpy(hgtstr, value, 3);
						int hgt = atoi(hgtstr);
						this_field_valid = (150 <= hgt) && (hgt <= 193);
					} else if (matches("00in", value)) {
						char* hgtstr = calloc(5, sizeof(char));
						strncpy(hgtstr, value, 2);
						int hgt = atoi(hgtstr);
						this_field_valid = (59 <= hgt) && (hgt <= 76);
					} else {
						this_field_valid = 0;
					}
					break;
				case 4: // hcl
					this_field_valid = matches("#xxxxxx", value);
					break;
				case 5: // ecl
					;
					int color_idx;
					for (color_idx = 0; color_idx < sizeof(colors)/sizeof(colors[0]); color_idx++) {
						if (strcmp(colors[color_idx], value) == 0) {
							this_field_valid = 1;
						}
					}
					break;
				case 6: // pid
					this_field_valid = matches("000000000", value);
					break;
				}

				printf("%s %s %d %d\n", field, value, field_idx, this_field_valid);

				valids[field_idx] = this_field_valid;
				token = strtok(NULL, " \n");
			}
		}
	}

	printf("VALID COUNT %d\n", valid_count);

	return 0;
}
