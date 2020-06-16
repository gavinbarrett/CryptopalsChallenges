#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <assert.h>
#include "aes.h"

struct AES_ctx ctx;
uint8_t key[] = "YELLOW SUBMARINE";

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

int main() {
	FILE* fp = fopen("10.txt","r");
	char* arr = malloc(4096);
	char* buff = malloc(64);
	
	while(fgets(buff, 64, fp)) {
		strcat(arr, buff);
		strtok(arr, "\n");
	}
	fclose(fp);

	AES_init_ctx(&ctx, key);
	
	int s = (int)strlen(arr)*(4/3.0);
	
	unsigned char* out = malloc(5120);
	size_t sz = (size_t)s;
	
	Base64Decode(arr, &out, &sz);

	decrypt(out, sz);
	
	free(arr);
	free(out);
	free(buff);
}
