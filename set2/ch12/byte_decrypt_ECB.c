#include <stdio.h>
#include <string.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <assert.h>
#include "aes.h"

struct AES_ctx ctx;
uint8_t key[17] = {0};
char *arr = "Um9sbGluJyBpbiBteSA1LjAKV2l0aCBteSByYWctdG9wIGRvd24gc28gbXkgaGFpciBjYW4gYmxvdwpUaGUgZ2lybGllcyBvbiBzdGFuZGJ5IHdhdmluZyBqdXN0IHRvIHNheSBoaQpEaWQgeW91IHN0b3A/IE5vLCBJIGp1c3QgZHJvdmUgYnkK";

size_t calcDecodeLength(const char* b64input) { //Calculates the length of a decoded string
	size_t len = strlen(b64input),
		padding = 0;

	if (b64input[len-1] == '=' && b64input[len-2] == '=') //last two chars are =
		padding = 2;
	else if (b64input[len-1] == '=') //last char is =
		padding = 1;

	return (len*3)/4 - padding;
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

void encrypt_buffer(char *buffer, unsigned char *base, uint8_t *ciphertext) {
	strcat(buffer, (char*)base);
	uint8_t buff[17] = {0};
	for (int i = 0; i < 16; i++)
		buff[i] = (uint8_t)buffer[i];
	ECB_round(buff, ciphertext);
}

int find_block_size(int sz, unsigned char* base) {
	uint8_t past_ctext[16];
	for (int i = 1; i < 128; i++) {
		char arr[i+sz+1];
		uint8_t ciphertext[17] = {0};
		int j = 0;
		for (; j < i+1; j++)
			arr[j] = 'A';
		arr[i] = 0;
		encrypt_buffer(arr, base, ciphertext);
		if (strncmp((char*)past_ctext, (char*)ciphertext, 16) == 0)
			return i-1;
		past_ctext[0] = '\0';
		strncpy((char*)past_ctext, (char*)ciphertext, 16);
		past_ctext[16] = '\0';
	}
	return 0;
}

void init_cipher() {
	int t = time(NULL);
	srand(t);
	for (int i = 0; i < 16; i++)
		key[i] = rand() % 256;
	AES_init_ctx(&ctx, key);
}

void attack_ecb(int blk_sz) {

}

int main() {
	// initialize the AES-ECB cipher
	init_cipher();
	int sz = (int)strlen(arr);
	sz = ((int)sz*(3/4.0)+1);
	unsigned char *base;
	size_t s = (size_t)sz;

	Base64Decode(arr, &base, &s);
	base[sz] = 0;
	// determine the block size
	int bs = find_block_size(sz, base);
	//char block[bs];
	//for (int i = 0; i < bs; i++)
	//	block[i] = 'A';
	//block[bs] = 0;
	if (bs > 0)
		printf("ECB mode detected!\nBlock size: %d\n", bs);
	
	// FIXME: create buffer of size bs-1 full of As
	// continually shorted buffer and secret
	// and check for similar blocks
	attack_ecb(bs);

	return 0;
}
