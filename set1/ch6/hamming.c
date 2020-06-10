#include <stdio.h>
#include <string.h>
#include "hamming.h"

int pop_count(char c) {
	int count = 0;
	while (c > 0) {
		if ((c & 0x1) == 1)
			count += 1;
		c >>= 1;
	}
	return count;
}

int hamming(char str1[], char str2[]) {
	int count = 0;
	for (int i = 0; i < (unsigned int)strlen(str1); i++)
		count += pop_count(str1[i] ^ str2[i]);
	return count;
}

/*
int main() {
	
	char* str1 = "this is a test";
	char* str2 = "wokka wokka!!!";
	
	printf("%d\n", hamming(str1, str2));

	return 0;
}
*/
