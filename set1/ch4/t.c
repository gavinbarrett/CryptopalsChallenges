#include <stdio.h>
#include "single_byte_xor.h"

int main(void) {

	char p[] = "hello\0";
	double score = score_code(p);
	printf("score: %f\n", score);

	return 0;
}
