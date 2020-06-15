#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <assert.h>
#include "hamming.h"

int calcDecodeLength(const char* b64input) { //Calculates the length of a decoded base64 string
  int len = strlen(b64input);
  int padding = 0;

  if (b64input[len-1] == '=' && b64input[len-2] == '=') //last two chars are =
    padding = 2;
  else if (b64input[len-1] == '=') //last char is =
    padding = 1;

  return (int)len*0.75 - padding;
}

int Base64Decode(char* b64message, unsigned char** buffer, size_t* length) { //Decodes a base64 encoded string
	BIO *bio, *b64;

	int decodeLen = calcDecodeLength(b64message);
	*buffer = (unsigned char*)malloc(decodeLen + 1);
	(*buffer)[decodeLen] = '\0';

	bio = BIO_new_mem_buf(b64message, -1);
	b64 = BIO_new(BIO_f_base64());
	bio = BIO_push(b64, bio);

	BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL); //Do not use newlines to flush buffer
	*length = BIO_read(bio, *buffer, strlen(b64message));
	assert(*length == decodeLen); //length should equal decodeLen, else something went horribly wrong
	BIO_free_all(bio);

	return (0); //success
}


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
		return 0.0;
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
		if (n_exp == 0)
			continue;
		float diff = (n_obs - n_exp);
		acc += (diff*diff) / n_exp;
	}
	return acc;
}

char break_cipher(char code[], char plaintext[]) {
	int sz = (int)strlen(alphabet);
	int clen = (int)strlen(code);
	float score = 0;
	float sc = 0.0;
	char key = 0;
	for (int i = 0; i < sz; i++) {
		int k = 0;
		char newcode[100] = {0};
		for (int j = 0; j < clen; j+=2) {
			newcode[k] = code[j] ^ alphabet[i];
			k++;
		}
		sc = score_code(newcode);
		if (sc > score) {
			score = sc;
			key = alphabet[i];
			strncpy(plaintext, newcode, 100);
		}
	}
	return key;
}

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

void encrypt(char string[], char key[]) {
	int k_length = (int)strlen(key);
	int index = 0;
	for (int i = 0; i < 2880; i++) {
		string[i] ^= key[index];
		printf(" %02X\n", key[index]);
		index = (index+1) % k_length;
	}
}

int main(void) {
	//char* str1 = "this is a test";
	//char* str2 = "wokka wokka!!!";
	//printf("%d\n", hamming(str1, str2));
	FILE* fp = fopen("6.txt","r");
	char* arr = malloc(3840);
	unsigned char* out = malloc(5120);
	char* buffer = malloc(60);
	while(fgets(buffer, 60, fp)) {
		strcat(arr, buffer);
		strtok(arr, "\n");
	}
	
	size_t sz = (size_t)5120;

	Base64Decode(arr, &out, &sz);
	
	char* nout = malloc(5120);
	for(int i = 0; i < 2880; i++)
		nout[i] = (char)out[i];
	
	double inf = DBL_MAX;
	int key = 0;
	for (int i = 2; i < 40; i++) {
		char* str1 = malloc(i);
		char* str2 = malloc(i);
		char* str3 = malloc(i);
		char* str4 = malloc(i);
		strncpy(str1, nout, i);
		nout += i;
		strncpy(str2, nout, i);
		nout += i;
		strncpy(str3, nout, i);
		nout += i;
		strncpy(str4, nout, i);
		nout -= (i*3);
		double one2 = (hamming(str1, str2)*1.0)/i;
		double one3 = (hamming(str1, str3)*1.0)/i;
		double one4 = (hamming(str1, str4)*1.0)/i;
		double one = (one2+one3+one4)/3;
		
		double two3 = (hamming(str2, str3)*1.0)/i;
		double two4 = (hamming(str2, str4)*1.0)/i;
		double two = (one2+two3+two4)/3;
		
		double three4 = (hamming(str3, str4)*1.0)/i;
		double three = (one3+two3+three4)/3;

		double four = (one4+two4+three4)/3;

		double h = (one+two+three+four)/4;

		if (h < inf) {
			inf = h;
			key = i;
		}
		free(str1);
		free(str2);
		free(str3);
		free(str4);
	}

	char blocks[100][100];
	int i = 0;
	int l = 0;
	for (; i < 2880; i+=key) {
		
		for (int j = 0; j < key; j++) {
			blocks[j][l] = nout[i+j];
		}
		l++;
	}
	/*
	for (int i = 0; i < key; i++) {
		printf("Block: ");
		for (int j = 0; j < l; j++)
			printf("%02X ", blocks[i][j]);
		printf("\n");
	}
	*/
	char plain[100];

	char* enc_key = malloc(key);

	for (int i = 0; i < key; i++) {
		key = break_cipher(blocks[i], plain);
		printf("BlockKey: %02X\n", key);
		enc_key[i] = key;
	}
	printf("\nMessage key: \'%s\'\n", enc_key);
	
	encrypt(nout, enc_key);
	printf("Plaintext: %s\n",nout);

	free(enc_key);
	free(arr);
	free(out);
	free(nout);
	free(buffer);
	return 0;
}
