#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

int main(int argc, char** argv) {
	if (argc != 2) {
		fprintf(stderr, "usage: %s input\n", argv[0]);
		exit(1);
	}

	FILE* in = fopen(argv[1], "r");

	char* line = calloc(100, sizeof(char));

	uint64_t *mem = calloc(100000, sizeof(uint64_t));
	int i;
	for (i = 0; i < 100000; i++) {
		mem[i] = 0ULL;
	}

	uint64_t andmask, ormask;
	int address = 0;
	uint64_t val = 0ULL;
	while (fgets(line, 100, in) != NULL) {
		if (line[1] == 'a') {
			andmask = 0ULL;
			ormask = 0ULL;
			for (i = 0; i <= 36; i++) {
				char c = line[strlen(line) - 1 - i];
				uint64_t i2 = 1ULL << (i - 1);
				if (c == 'X') {
					printf("Found unset bit 2^%d = %lu\n", i, i2);
					andmask |= i2;
				} else if (c == '0') {
				} else if (c == '1') {
					printf("Found set bit 2^%d = %lu\n", i, i2);
					andmask |= i2;
					ormask |= i2;
				}
			}
			printf("Setting andmask = %lu, ormask = %lu\n", andmask, ormask);
		} else if (line[1] == 'e') {
			int success = sscanf(line, "mem[%d] = %lu\n", &address, &val);
			if (success != 2) {
				fprintf(stderr, "Error parsing %s", line);
				exit(1);
			}
			val &= andmask;
			val |= ormask;
			printf("Writing %lu to %d\n", val, address);
			mem[address] = val;
		}
		printf("%s", line);
	}

	uint64_t total = 0ULL;
	for (i = 0; i < 100000; i++) {
		if (mem[i] != 0) {
			printf("%d %lu\n", i, mem[i]);
		}
		total += mem[i];
	}

	printf("TOTAL %lu\n", total);

	free(mem);
	free(line);


	return 0;
}
