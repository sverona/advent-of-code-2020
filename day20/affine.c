#include <stdlib.h>
#include <stdio.h>
#include <math.h>

void affine_transform(int rotation, int xflip, int array_size, int* x, int* y) {
	int sin[4] = {0, 1, 0, -1};
	int cos[4] = {1, 0, -1, 0};

	float x0 = (array_size - 1) / 2.0;
	float y0 = x0;

	int f = xflip ? -1 : 1;

	float xf = (*x - x0) * f * cos[rotation] - (*y - y0) * sin[rotation] + x0;
	float yf = (*x - x0) * f * sin[rotation] + (*y - y0) * cos[rotation] + y0;

	printf("(%d %d) %d %d -> %d %d\n", rotation, xflip, *x, *y, (int)round(xf), (int)round(yf));

	*x = (int)round(xf);
	*y = (int)round(yf);
}

int main(int argc, char** argv) {
	int i, j;
	int rot, flip;
	int size = 4;
	for (rot = 0; rot < 4; rot++) {
		for (flip = 0; flip <= 1; flip++) {
			for (i = 0; i < size; i++) {
				for (j = 0; j < size; j++) {
					int x = i, y = j;
					affine_transform(rot, flip, size, &x, &y);
				}
			}
		}
	}

	return 0;
}
