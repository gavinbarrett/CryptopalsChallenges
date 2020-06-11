#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

char alphabet[53] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',' '};

// probabilities source from http://pi.math.cornell.edu/~mec/2003-2004/cryptography/subs/frequencies.html
float freq_table[27] = {8.12,1.49,2.71,4.32,12.02,2.30,2.03,5.92,7.31,0.10,0.69,3.98,2.61,6.95,7.68,1.82,0.11,6.02,6.28,9.10,2.88,1.11,2.09,0.17,2.11,0.07,18.29};

int hti(unsigned char hex) {
	if ((hex > 96) && (hex < 103))
		return hex - 87;
	else
		return hex - 48;
}

char decode(char a, char b) {
	a = hti(a); b = hti(b);
	char c = a << 4;
	return (c |= b);
}

float compute_exp(int c, float n_obs) {
	if ((c > 96) && (c < 123)) {
		c -= 97;
		return freq_table[c];		
	} else if ((c > 64) && (c < 91)) {
		c -= 65;
		return freq_table[c];
	} else if (c == 32) {
		c -= 5;
		return freq_table[c];
	} else
		return n_obs;
}

float compute_obs(char c, char string[]) {
	float sz = (double)strlen(string);
	float acc = 0.0;
	for (int i = 0; i < sz; i++) {
		if (string[i] == c)
			acc++;
	}
	return acc / sz;
}

float score_code(char newcode[], float score) {
	int sz = (int)strlen(newcode);
	for (int i = 0; i < sz; i++) {
		float n_obs = compute_obs(newcode[i], newcode);
		float n_exp = compute_exp(newcode[i], n_obs);
		float diff = n_obs - n_exp;
		float e = 2.0;
		float p = powf(diff, e);
		score += (p / n_exp);
	}
	return score;
}

char break_cipher(char code[], char plaintext[]) {
	int sz = (int)strlen(alphabet);
	int clen = (int)strlen(code);
	float score = 0.0;
	float sc = 0.0;
	char key = 0;
	for (int i = 0; i < sz; i++) {
		int k = 0;
		char* newcode = malloc(clen);
		for (int j = 0; j < clen; j+=2) {
			newcode[k] = decode(code[j], code[j+1]) ^ alphabet[i];
			k++;
		}
		sc = score_code(newcode, 0.0);
		if ((sc > score) && (score != (1/0.0))) {
			score = sc;
			key = alphabet[i];
			strncpy(plaintext, newcode, clen);
		}
		free(newcode);
	}
	return key;
}
