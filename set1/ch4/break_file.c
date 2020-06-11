#include <stdio.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
//#include "single_byte_xor.h"

char alphabet[53] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',' '};

// probabilities source from http://pi.math.cornell.edu/~mec/2003-2004/cryptography/subs/frequencies.html
float freq_table[27] = {8.12,1.49,2.71,4.32,12.02,2.30,2.03,5.92,7.31,0.10,0.69,3.98,2.61,6.95,7.68,1.82,0.11,6.02,6.28,9.10,2.88,1.11,2.09,0.17,2.11,0.07,18.29};

int hti(unsigned char hex) {
	if ((hex > 96) && (hex < 103))
		return hex - 87;
	else
		return hex - 48;
}

char dec(char a, char b) {
	a = hti(a); b = hti(b);
	char c = a << 4;
	return (c |= b);
}

double comp_exp(int c, int len, double n_obs) {
	if ((c > 96) && (c < 123)) {
		c -= 97;
		return freq_table[c] * len;		
	} else if ((c > 64) && (c < 91)) {
		c -= 65;
		return freq_table[c] * len;
	} else if (c == 32) {
		c -= 5;
		return freq_table[c];
	} else
		return n_obs;
}

double comp_obs(char c, char string[]) {
	double sz = (double)strlen(string);
	double acc = 0.0;
	for (int i = 0; i < sz; i++) {
		if (string[i] == c)
			acc++;
	}
	return acc / sz;
}

double sc(char newcode[]) {
	double acc = 0.0;
	int sz = (int)strlen(newcode);
	for (int i = 0; i < sz; i++) {
		double n_obs = comp_obs(newcode[i], newcode);
		double n_exp = comp_exp(newcode[i], sz, n_obs);
		if (n_exp == 0.0)
			continue;
		double diff = (n_obs - n_exp);
		acc += ((diff*diff) / n_exp);
	}
	return acc;
}

char ciph(char code[], char plaintext[], char x) {
	int sz = (int)strlen(alphabet);
	int clen = (int)strlen(code);
	float score = 0.0;
	float sc_code = 0.0;
	char key = 0;
	for (int i = 0; i < sz; i++) {
		int k = 0;
		char* newcode = malloc(clen);
		for (int j = 0; j < clen; j+=2) {
			newcode[k] = dec(code[j], code[j+1]) ^ alphabet[i];
			k++;
		}
		sc_code = sc(newcode);
		//printf("%f\n", sc_code);
		if (sc_code > score) {
			//printf("Char %c for %d: %s\n", alphabet[i], x, newcode);
			score = sc_code;
			key = alphabet[i];
			strncpy(plaintext, newcode, clen);
		}
		free(newcode);
	}
	return key;
}

int main(void) {
	FILE* fp = fopen("file.txt", "r");
	char buffer[62];
	char plaintext[62] = {0};
	char splain[62] = {0};
	double min = -100.0;
	double score = 0.0;
	int i = 0;
	int n = 0;
	char key = 0;
	char k = 0;
	while (fgets(buffer, 62, fp) != NULL) {
		key = ciph(buffer, plaintext, i);
		score = sc(plaintext);
		printf("%s\n", plaintext);
		//printf("%f\n", score);
		if (score > min) {
			min = score;
			strncpy(splain, plaintext, 62);
			k = key;
			n = i;
		}
		i++;
	}
	printf("Plaintext: %s\n", splain);
	printf("line: %d\n", n);
	printf("key: %c\n", k);
	return 0;
}
