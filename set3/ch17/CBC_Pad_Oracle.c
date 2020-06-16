#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <inttypes.h>
#include "aes.h"

#define BLOCK_SZ 16

struct AES_ctx ctx;

char* messages[10] = {
	"MDAwMDAwTm93IHRoYXQgdGhlIHBhcnR5IGlzIGp1bXBpbmc=",
	"MDAwMDAxV2l0aCB0aGUgYmFzcyBraWNrZWQgaW4gYW5kIHRoZSBWZWdhJ3MgYXJlIHB1bXBpbic=",
	"MDAwMDAyUXVpY2sgdG8gdGhlIHBvaW50LCB0byB0aGUgcG9pbnQsIG5vIGZha2luZw==",
	"MDAwMDAzQ29va2luZyBNQydzIGxpa2UgYSBwb3VuZCBvZiBiYWNvbg==",
	"MDAwMDA0QnVybmluZyAnZW0sIGlmIHlvdSBhaW4ndCBxdWljayBhbmQgbmltYmxl",
	"MDAwMDA1SSBnbyBjcmF6eSB3aGVuIEkgaGVhciBhIGN5bWJhbA==",
	"MDAwMDA2QW5kIGEgaGlnaCBoYXQgd2l0aCBhIHNvdXBlZCB1cCB0ZW1wbw==",
	"MDAwMDA3SSdtIG9uIGEgcm9sbCwgaXQncyB0aW1lIHRvIGdvIHNvbG8=",
	"MDAwMDA4b2xsaW4nIGluIG15IGZpdmUgcG9pbnQgb2g=",
	"MDAwMDA5aXRoIG15IHJhZy10b3AgZG93biBzbyBteSBoYWlyIGNhbiBibG93"
};

void pad(char* string) {
	int len = (int)strlen(string);
	int off = (len % BLOCK_SZ);
	if (off > 0)
		off = BLOCK_SZ - off;
	int i = len;
	for (; i < len+off; i++) {
		string[i] = off;
	}
}

void encipher_block(uint8_t* in, uint8_t* out) {
	for (int i = 0; i < 16; i++)
		out[i] = in[i];
	AES_ECB_encrypt(&ctx, out);
}

void decipher_block(uint8_t* in, uint8_t* out) {
	for (int i = 0; i < 16; i++)
		out[i] = in[i];
	AES_ECB_decrypt(&ctx, out);
}


void xor_block(uint8_t enc_blk[], uint8_t* ciphertext) {
	for (int i = 0; i < 16; i++)
		ciphertext[i] ^= enc_blk[i];
}

void encrypt(unsigned char* in, unsigned char* out, size_t sz, uint8_t* iv) {
	int blk_i = 0;
	uint8_t enc_blk[17] = {0};
	uint8_t ciph[17] = {0};
	for (int i = 0; i < 16; i++)
		iv[i] = ciph[i];
	for (int i = 0; i < (int)sz; i++) {
		if (blk_i == 16) {
			blk_i = 0;
			// xor block
			xor_block(enc_blk, ciph);
			// decipher block
			encipher_block(ciph, ciph);
			strcat((char*)out, (char*)ciph);
		}
		enc_blk[blk_i] = in[i];
		blk_i++;
	}
	xor_block(enc_blk, ciph);
	encipher_block(ciph, ciph);
	strcat((char*)out, (char*)ciph);
}

void decrypt(unsigned char* out, size_t sz) {
	int blk_i = 0;
	uint8_t enc_blk[17] = {0};
	uint8_t ciph[17] = {0};
	uint8_t dec[17] = {0};
	for (int i = 0; i < (int)sz; i++) {
		if (blk_i == 16) {
			blk_i = 0;
			// decipher block
			decipher_block(enc_blk, dec);
			// xor block
			xor_block(dec, ciph);
			printf("%s", ciph);
			for (int j = 0; j < 16; j++)
				ciph[j] = enc_blk[j];
		}
		enc_blk[blk_i] = out[i];
		blk_i++;
	}
	decipher_block(enc_blk, dec);
	xor_block(dec, ciph);
	printf("%s\n", ciph);
}

int check_validity(char str[]) {
	int sz = (int)strlen(str);
	char pad_byte = str[sz-1];
	if (pad_byte > 0x0f)
		return 1;
	for (int i = sz-1; i > sz-((int)pad_byte)-1; i--) {
		if (str[i] == pad_byte)
			continue;
		else
			return 0;
	}
	return 1;
}

int valid_pad(char* ciphertext, size_t size) {
	// decrypt the ciphertext
	decrypt((unsigned char*)ciphertext, size);
	// check correct PKCS padding
	return check_validity(ciphertext);
}

int main() {
	// get time
	int n = time(NULL);
	
	// seed the generator
	srand(n);
	
	// get a random number
	int r = rand() % 10;
	
	char plaintext[1000] = {0};
	strcpy(plaintext, messages[r]);

	uint8_t key[16];

	// get random byte string
	for (int i = 0; i < 16; i++)
		key[i] = rand() % 256;
	
	AES_init_ctx(&ctx, key);

	// pad the string
	pad(plaintext);

	size_t sz = (size_t)strlen(plaintext);
	
	char ciphertext[1000] = {0};
	uint8_t iv[17] = {0};

	encrypt((unsigned char*)plaintext, (unsigned char*)ciphertext, sz, iv);

	//printf("%s\n", ciphertext);
	decrypt((unsigned char*)ciphertext, sz);

	if (valid_pad(ciphertext, sz))
		printf("\nValid PKCS padding!");

	return 0;
}
