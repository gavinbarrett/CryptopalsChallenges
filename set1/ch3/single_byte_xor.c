#include <stdio.h>
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

float compute_exp(int c, int len, float n_obs) {
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

float compute_obs(char c, char string[]) {
	int sz = (int)strlen(string);
	float acc = 0;
	for (int i = 0; i < sz; i++) {
		if (string[i] == c)
			acc++;
	}
	return acc / sz;
}

float score_code(char newcode[]) {
	float acc = 0;
	int sz = (int)strlen(newcode);
	for (int i = 0; i < sz; i++) {
		float n_obs = compute_obs(newcode[i], newcode);
		float n_exp = compute_exp(newcode[i], sz, n_obs);
		acc += (pow((n_obs - n_exp), 2.0) / n_exp);
	}
	return acc;
}

int main() {

	char code[69] = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
	
	int sz = (int)strlen(alphabet);
	int clen = (int)strlen(code);
	float score = 0;
	char plaintext[35] = {0};
	float sc = 0.0;
	char key = 0;
	for (int i = 0; i < sz; i++) {
		int k = 0;
		char newcode[35] = {0};
		for (int j = 0; j < clen; j+=2) {
			newcode[k] = decode(code[j], code[j+1]) ^ alphabet[i];
			k++;
		}
		sc = score_code(newcode);
		if (sc > score) {
			score = sc;
			key = alphabet[i];
			strncpy(plaintext, newcode, 34);
		}
	}
	if (key == 'X') {
		printf("\nCipher broken!\n");
		printf("\nPlaintext:\n%s\n", plaintext);
	} else
		printf("cipher wasn't broken");
	return 0;
}
