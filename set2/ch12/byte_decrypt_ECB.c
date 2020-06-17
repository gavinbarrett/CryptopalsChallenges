#include <stdio.h>
#include <string.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <assert.h>
#include "aes.h"

struct AES_ctx ctx;
uint8_t key[17] = {0};

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
	printf("%s", ciphertext);
}

void CBC_round(uint8_t *enc_blk, uint8_t *ciphertext) {
	// xor block
	xor_block(enc_blk, ciphertext);
	// encipher block
	ECB_round(enc_blk, ciphertext);
}

void encrypt(unsigned char* out, size_t sz, int mode) {
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
		}
		enc_blk[blk_i] = out[i];
		blk_i++;
	}
	if (!mode)
		ECB_round(enc_blk, ciph);
	else
		CBC_round(enc_blk, ciph);
}

int main() {
	int t = time(NULL);
	srand(t);
	unsigned char *out = malloc(2000);
	char *arr = "Um9sbGluJyBpbiBteSA1LjAKV2l0aCBteSByYWctdG9wIGRvd24gc28gbXkgaGFpciBjYW4gYmxvdwpUaGUgZ2lybGllcyBvbiBzdGFuZGJ5IHdhdmluZyBqdXN0IHRvIHNheSBoaQpEaWQgeW91IHN0b3A/IE5vLCBJIGp1c3QgZHJvdmUgYnkK";
	
	char input[2000] = "hello there";
	size_t sz = (size_t)2000;
	// decode the b64 string
	Base64Decode(arr, &out, &sz);
	
	// concatenate string and decoded base64
	strcat(input, (char*)out);
	free(out);		
	
	// randomly generate a key
	for (int i = 0; i < 16; i++)
		key[i] = rand() % 256;
	// initialize AES object
	AES_init_ctx(&ctx, key);

	encrypt((unsigned char*)input, sz, 0);
	// FIXME: encrypt the input string with AES-ECB

	return 0;
}
