#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {

	int last_iter = atoi(argv[1]);
	int *cache = calloc(last_iter, sizeof(int));

	cache[1] = 1;
	cache[20] = 2;
	cache[8] = 3;
	cache[12] = 4;
	cache[0] = 5;
	cache[14] = 6;

	int num = 14;
	int next = 0;
	int time = 6;
	for (time = 6; time <= last_iter; time++) {
		if (cache[num] == 0) {
			next = 0;
		} else {
			next = time - cache[num];
		}
		if (time == last_iter) {
			printf("%d %d %d\n", time, num, cache[num]);
		}
		cache[num] = time;
		num = next;
	}

	free(cache);

	return 0;
}
