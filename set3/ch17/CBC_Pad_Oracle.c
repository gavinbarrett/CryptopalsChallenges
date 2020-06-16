#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
	char pad = 0x04;
	int len = (int)strlen(string);
	int off = BLOCK_SZ - (len % BLOCK_SZ) + 1;
	int i = len;
	for (; i < len+off-1; i++) {
		string[i] = pad;
	}
	string[i] = 0;
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

void encrypt(unsigned char* in, unsigned char* out, size_t sz) {
	int blk_i = 0;
	uint8_t enc_blk[17] = {0};
	uint8_t ciph[17] = {0};
	//uint8_t dec[17] = {0};
	for (int i = 0; i < (int)sz; i++) {
		if (blk_i == 16) {
			blk_i = 0;
			// xor block
			xor_block(enc_blk, ciph);
			// decipher block
			encipher_block(ciph, ciph);
			strcat((char*)out, (char*)ciph);
			//printf("%s", ciph);
			//for (int j = 0; j < 16; j++)
			//	ciph[j] = enc_blk[j];
		}
		enc_blk[blk_i] = in[i];
		blk_i++;
	}
	xor_block(enc_blk, ciph);
	encipher_block(ciph, ciph);
	strcat((char*)out, (char*)ciph);
	//printf("%s\n", ciph);
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

int main() {
	// get time
	int n = time(NULL);
	
	// seed the generator
	srand(n);
	
	// get a random number
	int r = rand() % 10;
	
	char arr[1000] = {0};
	strcpy(arr, messages[r]);

	uint8_t key[16];

	// get random byte string
	for (int i = 0; i < 16; i++)
		key[i] = rand() % 256;
	
	AES_init_ctx(&ctx, key);

	// pad the string
	pad(arr);

	size_t sz = (size_t)strlen(arr);
	char out[1000] = {0};
	encrypt((unsigned char*)arr, (unsigned char*)out, sz);
	printf("%s\n", out);

	decrypt((unsigned char*)out, sz);

	return 0;
}
