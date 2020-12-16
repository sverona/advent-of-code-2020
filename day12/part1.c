#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char** argv) {
	if (argc != 2) {
		fprintf(stderr, "usage: %s input\n", argv[0]);
		exit(1);
	}

	FILE* in = fopen(argv[1], "r");

	char* line = calloc(100, sizeof(char));
	char ins;
	int val;

	int x = 0;
	int y = 0;
	int dirx = 1;
	int diry = 0;
	while (fgets(line, 100, in) != NULL) {
		int success = sscanf(line, "%[NSEWLRF]%d\n", &ins, &val);
		if (success != 2) {
			fprintf(stderr, "(%d) couldn't parse %s", success, line);
			exit(1);
		}
		float rad;
		int vsin, vcos, newdiry, newdirx;
		switch (ins) {
		case 'N':
			y += val;
		break;
		case 'S':
			y -= val;
		break;
		case 'E':
			x += val;
		break;
		case 'W':
			x -= val;
		break;
		case 'L':
			;
			rad = val * M_PI / 180;
			vsin = (int)round(sin(rad));
			vcos = (int)round(cos(rad));
			newdiry = diry * vcos + dirx * vsin;
			newdirx = dirx * vcos - diry * vsin;
			diry = newdiry;
			dirx = newdirx;
		break;
		case 'R':
			;
			rad = val * M_PI / 180;
			vsin = (int)round(sin(rad));
			vcos = (int)round(cos(rad));
			newdiry = diry * vcos - dirx * vsin;
			newdirx = dirx * vcos + diry * vsin;
			diry = newdiry;
			dirx = newdirx;
		break;
		case 'F':
			x += dirx * val;
			y += diry * val;
		break;
		}
		printf("%s", line);
		printf("pos:%d %d  dir:%d %d\n", x, y, dirx, diry);
	}

	free(line);

	return 0;
}
