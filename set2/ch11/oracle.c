#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "aes.h"

struct AES_ctx ctx;

void encipher_block(uint8_t *in, uint8_t *out) {
	for (int i = 0; i < 16; i++)
		out[i] = in[i];
	AES_ECB_encrypt(&ctx, out);
}

void xor_block(uint8_t *enc_blk, uint8_t *ciphertext) {
	for (int i = 0; i < 16; i++)
		enc_blk[i] ^= ciphertext[i];
}

void ECB_round(uint8_t *enc_blk, uint8_t *ciphertext) {
	// encipher block
	encipher_block(enc_blk, ciphertext);
	//printf("%s", ciphertext);
	// iterate through the plaintext and the cipher text. for each
}

void CBC_round(uint8_t *enc_blk, uint8_t *ciphertext) {
	// xor block
	xor_block(enc_blk, ciphertext);
	// encipher block
	ECB_round(enc_blk, ciphertext);
}

void encrypt(unsigned char* in, unsigned char* out, size_t sz, int mode) {
	int blk_i = 0;
	uint8_t enc_blk[17] = {0};
	uint8_t ciph[17] = {0};
	for (int i = 0; i < (int)sz; i++) {
		if (blk_i == 16) {
			blk_i = 0;
			if (!mode)
				ECB_round(enc_blk, ciph);
			else
				CBC_round(enc_blk, ciph);
			strcat((char*)out,(char*)ciph);
		}
		enc_blk[blk_i] = in[i];
		blk_i++;
	}
	if (!mode)
		ECB_round(enc_blk, ciph);
	else
		CBC_round(enc_blk, ciph);
	strcat((char*)out,(char*)ciph);
}

int encryption_oracle(unsigned char *message) {
	int seed = time(NULL);
	unsigned char key[16];
	//unsigned char ht[] = "hello there gavin barrett";
	
	srand(seed);
	
	for (int i = 0; i < 16; i++)
		key[i] = rand() % 256;

	AES_init_ctx(&ctx, key);
	
	int mode = rand() % 2;
	printf("%d\n", mode);
	int sz = (int)strlen((char*)message);
	unsigned char *out = malloc(sz);
	encrypt(message, out, (size_t)sz, mode);
	unsigned char *half1 = malloc(sz/2);
	unsigned char *half2 = malloc(sz/2);
	
	strncpy((char*)half1, (char*)out, sz/2);
	strncpy((char*)half2, (char*)out+(sz/2), sz/2);

	printf("%s\n\n", half1);
	printf("%s\n", half2);
	
	if (strcmp((char*)half1,(char*)half2) == 0) {
		free(half1);
		free(half2);
		return 0;
	}
	free(half1);
	free(half2);
	return 1;
}

int main() {
	unsigned char message[33] = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
	int r = encryption_oracle(message);
	if (r)
		printf("CBC oracle detected\n");
	else
		printf("ECB oracle detected\n");
	return 0;
}
