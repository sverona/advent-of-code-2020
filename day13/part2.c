#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long modular_inverse(long a, long m) {
	long r = a % m;
	long r_old = m;
	long s = 1;
	long s_old = 0;
	long t = 0;
	long t_old = 1;
	long r_new, s_new, t_new;
	while (r > 0) {
		long q = r_old / r;
		r_new = r_old - q * r;
		s_new = s_old - q * s;
		t_new = t_old - q * t;

		r_old = r;
		r = r_new;

		s_old = s;
		s = s_new;
		
		t_old = t;
		t = t_new;
	}
	return s_old;
}

int main(int argc, char** argv) {
	if (argc != 2) {
		fprintf(stderr, "usage: %s input\n", argv[0]);
		exit(1);
	}

	FILE* in = fopen(argv[1], "r");

	char* line = calloc(300, sizeof(char));
	char* token = calloc(10, sizeof(char));
	int i = 0;
	int time_index = 0;
	int bus_count = 0;
	int active_count = 0;

	while (fgets(line, 300, in) != NULL) {
		if (i++ == 0) {
			time_index = atoi(line);
		} else {
			token = strtok(line, ",");
			do {
				bus_count++;
				if (token[0] != 'x') {
					active_count++;
				}
			} while ((token = strtok(NULL, ",")) != NULL);
		}
	}

	int *buses = calloc(bus_count, sizeof(int));
	rewind(in);
	i = 0;
	while (fgets(line, 300, in) != NULL) {
		if (i++ == 1) {
			int bus_idx = 0;
			token = strtok(line, ",");
			do {
				if (token[0] == 'x') {
					buses[bus_idx++] = -1;
				} else {
					buses[bus_idx++] = atoi(token);
				}
			} while ((token = strtok(NULL, ",")) != NULL);
		}
	}

	int *remainders = calloc(active_count, sizeof(int));
	int *mods = calloc(active_count, sizeof(int));

	int j = 0;
	long product = 1;
	for (i = 0; i < bus_count; i++) {
		if (buses[i] >= 0) { 
			mods[j] = buses[i];
			product *= buses[i];
			remainders[j] = (buses[i] - i) % mods[j];
			j++;
		}
	}
	printf("PRODUCT\t%ld\n", product);

	long simult_index = 0;
	for (i = 0; i < active_count; i++) {
		long N = product / mods[i];
		long M = modular_inverse(N, mods[i]);
		while (M < 0) {
			M += mods[i];
		}
		simult_index += N * M * remainders[i];
	}
	printf("TIME   \t%ld\n", simult_index % product);

	free(line);
	free(token);

	return 0;
}
