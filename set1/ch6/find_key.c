#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include "hamming.h"

void key() {
	char a[40] = "abcdefghijasdffgdssesasfeffftggrjiikef";
	double l = DBL_MAX;
	int k = 0;
	for (int i = 2; i < 20; i++) {
		printf("i: %d\n", i);
		for (int j = 0; j < (int)strlen(a)/2; j+=(2*i)) {
			printf("j: %d\n", j);
			char* s1 = malloc(i);
			char* s2 = malloc(i);
			strncpy(s1, a+j, i);
			printf("s1: %s\n", s1);
			strncpy(s2, a+(j+i), i);
			printf("s2: %s\n", s2);
			double res = ((double)hamming(s1, s2)) / i;
			printf("res: %f\n", res);
			if (res < l) {
				l = res;
				k = i;
			}
			free(s1);
			free(s2);
		}
	}
	printf("key: %d", k);
}

int main(void) {
	//char* str1 = "this is a test";
	//char* str2 = "wokka wokka!!!";
	//printf("%d\n", hamming(str1, str2));
	key();
	return 0;
}
