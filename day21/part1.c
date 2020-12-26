#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv) {
	if (argc != 2) {
		fprintf(stderr, "usage: %s input\n", argv[0]);
		exit(1);
	}

	FILE* in = fopen(argv[1], "r");

	char* line = calloc(1000, sizeof(char));
	char* token = calloc(100, sizeof(char));
	char (*blocks)[20] = calloc(300, sizeof(*blocks));
	int end = 0;
	int num_lines = 0;
	while (fgets(line, 1000, in)) {
		num_lines++;
		token = strtok(line, " ");

		while (token && token[0] != '(') {
			int new = 1;
			int i;
			for (i = 0; i < end; i++) {
				if (strcmp(token, blocks[i]) == 0) {
					new = 0;
					break;
				}
			}
			if (new) {
				printf("blocks[%d] = %s\n", end, token);
				strcpy(blocks[end++], token);
			}

			token = strtok(NULL, " ");
		}
	}
	int num_blocks = end;

	rewind(in);
	char (*allergens)[20] = calloc(10, sizeof(*blocks));
	end = 0;
	while (fgets(line, 1000, in)) {
		token = strtok(line, "(");
		token = strtok(NULL, " "); // contains
		while (token = strtok(NULL, " ,)\n")) {
			int new = 1;
			int i;
			for (i = 0; i < end; i++) {
				if (strcmp(token, allergens[i]) == 0) {
					new = 0;
					break;
				}
			}
			if (new) {
				printf("allergens[%d] = %s\n", end, token);
				strcpy(allergens[end++], token);
			}

			token = strtok(NULL, " ");
		}
	}
	int num_allergens = end;

	rewind(in);
	int (*blocks_map)[num_blocks] = calloc(num_lines, sizeof(*blocks_map));
	int (*allergens_map)[num_allergens] = calloc(num_lines, sizeof(*allergens_map));
	int line_no = 0;
	while (fgets(line, 1000, in)) {
		int parsing_allergens = 0;
		token = strtok(line, " (),\n");
		do {
			if (strcmp(token, "contains") == 0) {
				parsing_allergens = 1;
				continue;
			}

			if (parsing_allergens) { 
				int allergen_id ;
				for (allergen_id = 0; allergen_id < num_allergens; allergen_id++) {
					if (strcmp(allergens[allergen_id], token) == 0) {
						allergens_map[line_no][allergen_id] = 1;
					}
				}
			} else {
				int block_id;
				for (block_id = 0; block_id < num_blocks; block_id++) {
					if (strcmp(blocks[block_id], token) == 0) {
						blocks_map[line_no][block_id] = 1;
					}
				}
			}

		} while (token = strtok(NULL, " (),\n"));
		line_no++;
	}

	int (*blocks_allergens_map)[num_blocks] = calloc(num_allergens, sizeof(*blocks_allergens_map));

	int block_id, allergen_id;
	char (*allergenics)[10] = calloc(num_allergens, sizeof(*allergenics));
	end = 0;

	for (block_id = 0; block_id < num_blocks; block_id++) {
		int allergenic = 0;
		for (allergen_id = 0; allergen_id < num_allergens; allergen_id++) {
			blocks_allergens_map[allergen_id][block_id] = 1;
			for (line_no = 0; line_no < num_lines; line_no++) {
				int allergen_here = allergens_map[line_no][allergen_id];
				int block_here = blocks_map[line_no][block_id];

				if (allergen_here && !block_here) {
					blocks_allergens_map[allergen_id][block_id] = 0;
				}
			}
			if (blocks_allergens_map[allergen_id][block_id]) {
				printf("%s contains %s\n", blocks[block_id], allergens[allergen_id]);
				allergenic = 1;
			}
		}

		if (allergenic) {
			strcpy(allergenics[end++], blocks[block_id]);
		}
	}

	rewind(in);
	int total_block_count = 0;
	while (fgets(line, 1000, in)) {
		token = strtok(line, " ");
		do {
			if (token[0] == '(') {
				break;
			}
			int i;
			int allergenic = 0;
			for (i = 0; i < num_allergens; i++) {
				if (strcmp(allergenics[i], token) == 0) {
					allergenic = 1;
				}
			}
			if (!allergenic) {
				total_block_count++;
			}
		} while (token = strtok(NULL, " "));
	}


	printf("TOTAL NONALLERGENIC %d\n", total_block_count);
	

	free(blocks_map);
	free(blocks_allergens_map);
	free(allergens_map);
	free(allergens);
	free(blocks);
	free(line);

	return 0;
}
