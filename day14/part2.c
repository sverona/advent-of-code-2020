#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct {
	uint64_t addr;
	int val;
} Write;

int main(int argv, char** argc) {
	FILE* in = fopen(argc[1], "r");

	char* line = calloc(100, sizeof(char));
	int accessed_spaces = 0;
	int floating_bits = 0;
	int i;
	while (fgets(line, 100, in) != NULL) {
		if (line[1] == 'a') {
			floating_bits = 0;
			for (i = 0; i < strlen(line); i++) {
				if (line[i] == 'X') {
					floating_bits++;
				}
			}
		} else if (line[1] == 'e') {
			accessed_spaces += 1 << floating_bits;
		}
	}

	printf("TOTAL SPACES NEEDED %d\n", accessed_spaces);

	Write *mem = calloc(accessed_spaces, sizeof(Write));

	rewind(in);
	int *bitmask = calloc(36, sizeof(int));
	int this_space = 0;
	while (fgets(line, 100, in) != NULL) {
		if (line[1] == 'a') {
			floating_bits = 0;
			for (i = 0; i < 36; i++) {
				switch (line[strlen(line) - 2 - i]) {
				case 'X':
					floating_bits++;
					bitmask[36 - 1 - i] = 2;
					break;
				case '1':
					bitmask[36 - 1 - i] = 1;
					break;
				case '0':
					bitmask[36 - 1 - i] = 0;
					break;
				}
			}
		} else if (line[1] == 'e') {
			uint64_t address;
			int val;
			int success = sscanf(line, "mem[%llu] = %d\n", &address, &val);
			if (success != 2) {
				fprintf(stderr, "Error parsing %s", line);
				exit(1);
			}

			int total_spaces = 1 << floating_bits;
			for (i = 0; i < total_spaces; i++) {
				uint64_t addr = address;
				int overwrite = i;
				int j;
				for (j = 0; j < 36; j++) {
					switch (bitmask[36 - 1 - j]) {
					case 0:
						break;
					case 1:
						addr |= (1ULL) << j;
						break;
					case 2:
						;
						uint64_t mask_bit = overwrite & 1ULL;
						overwrite >>= 1;
						if (mask_bit == 1) {
							addr |= (1ULL) << j;
						} else {
							addr &= ~(1ULL << j);
						}
						break;
					}
				}
				// printf("%llu -> %d\n", addr, val);
				int this_index = -1;
				for (j = 0; j < this_space; j++) {
					if (mem[j].addr == addr) {
						this_index = j;
						break;
					}
				}
				if (this_index == -1) {
					mem[this_space].addr = addr;
					mem[this_space].val = val;
					this_space++;
				} else {
					mem[this_index].val = val;
				}
			}
		}
	}

	uint64_t total = 0ULL;
	for (i = 0; i < this_space; i++) {
		total += mem[i].val;
	}
	printf("TOTAL %llu\n", total);


	free(mem);
	free(line);

	return 0;
}
