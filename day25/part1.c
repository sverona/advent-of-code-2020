#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {

	int card_pk = 9033205;
	// int card_pk = 17807724;	
	int door_pk = 9281649;

	int subject = 7;
	int size;
	long long val = 1;
	for (size = 0; ; size++) {
		// printf("step %d, val %d\n", size, val);
		if (val == card_pk) {
			break;
		}
		val = (val * subject) % 20201227;
	}
	int card_size = size;
	printf("Card loop size is %d\n", size);

	val = 1;
	for (size = 0; ; size++) {
		// printf("step %d, val %d\n", size, val);
		if (val == door_pk) {
			break;
		}
		val = (val * subject) % 20201227;
	}
	int door_size = size;
	printf("Door loop size is %d\n", size);

	subject = door_pk;
	val = 1;
	for (size = 0; size < card_size; size++) {
		val = (val * subject) % 20201227;
	}
	printf("Card gets %d\n", val);

	subject = card_pk;
	val = 1;
	for (size = 0; size < door_size; size++) {
		val = (val * subject) % 20201227;
	}
	printf("Door gets %d\n", val);

	return 0;
}
