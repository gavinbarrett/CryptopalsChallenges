#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <aes.h>

#define BLOCK_SZ 16

struct AES_ctx ctx;

void decipher_block(uint8_t* in, uint8_t* out) {
	for (int i = 0; i < 16; i++)
		out[i] = in[i];
	AES_ECB_decrypt(&ctx, out);
}

void xor_block(uint8_t enc_blk[], uint8_t* ciphertext) {
	for (int i = 0; i < 16; i++)
		ciphertext[i] ^= enc_blk[i];
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

int pk7_valid(char str[]) {
	int sz = (int)strlen(str);
	char pad_byte = str[sz-1];
	if (pad_byte > 0xff)
		return 1;
	for (int i = sz-1; i > sz-((int)pad_byte)-1; i--) {
		if (str[i] == pad_byte)
			continue;
		else
			return 0;
	}
	return 1;
}

void pk7_pad(char* string) {
	int len = (int)strlen(string);
	int off = (len % BLOCK_SZ);
	if (off > 0)
		off = BLOCK_SZ - off;
	int i = len;
	for (; i < len+off; i++)
		string[i] = off;
}
int main(void) {
	
	return 0;
}
