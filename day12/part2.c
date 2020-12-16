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
	int wx = 10;
	int wy = 1;
	while (fgets(line, 100, in) != NULL) {
		int success = sscanf(line, "%[NSEWLRF]%d\n", &ins, &val);
		if (success != 2) {
			fprintf(stderr, "(%d) couldn't parse %s", success, line);
			exit(1);
		}
		float rad;
		int vsin, vcos, newy, newx;
		switch (ins) {
		case 'N':
			wy += val;
		break;
		case 'S':
			wy -= val;
		break;
		case 'E':
			wx += val;
		break;
		case 'W':
			wx -= val;
		break;
		case 'L':
			;
			rad = val * M_PI / 180;
			vsin = (int)round(sin(rad));
			vcos = (int)round(cos(rad));
			newy = wy * vcos + wx * vsin;
			newx = wx * vcos - wy * vsin;
			wy = newy;
			wx = newx;
		break;
		case 'R':
			;
			rad = val * M_PI / 180;
			vsin = (int)round(sin(rad));
			vcos = (int)round(cos(rad));
			newy = wy * vcos - wx * vsin;
			newx = wx * vcos + wy * vsin;
			wy = newy;
			wx = newx;
		break;
		case 'F':
			x += wx * val;
			y += wy * val;
		break;
		}
		printf("%s", line);
		printf("pos:%d %d  way:%d %d\n", x, y, wx, wy);
	}


	free(line);
	printf("DISTANCE %d\n", (int)(abs(x) + abs(y)));

	return 0;
}
