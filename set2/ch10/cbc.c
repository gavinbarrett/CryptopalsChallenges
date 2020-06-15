#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "aes.h"

struct AES_ctx ctx;

void encipher(uint8_t block[], uint8_t* out) {
	uint8_t key[] = "YELLOW SUBMARINE";
	for (int i = 0; i < 16; i++) {
		out[i] = block[i];
	}
	AES_init_ctx(&ctx, key);
	AES_ECB_decrypt(&ctx, out);
}

void xor_block(uint8_t enc_blk[], uint8_t* ciphertext) {
	for (int i = 0; i < 16; i++)
		enc_blk[i] ^= ciphertext[i];
}

int main() {
	FILE* fp = fopen("10.txt","r");
	char* arr = malloc(4096);
	char* buff = malloc(64);
	while(fgets(buff, 64, fp)) {
		strcat(arr, buff);
		strtok(arr, "\n");
	}
	fclose(fp);
	int blk_i = 0;
	uint8_t enc_blk[16];
	uint8_t ciph[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	for (int i = 0; i < (int)strlen(arr); i++) {
		if (blk_i == 16) {
			blk_i = 0;
			xor_block(enc_blk, ciph);
			encipher(enc_blk, ciph);
			for (int i = 0; i < 16; i++)
				printf("%c ", (unsigned char)ciph[i]);
		}
		enc_blk[blk_i] = arr[i];
		blk_i++;
	}
	xor_block(enc_blk, ciph);
	encipher(enc_blk, ciph);
	printf("%s\n", ciph);
	free(arr);
	free(buff);
}
