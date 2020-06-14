#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <assert.h>
#include "aes.h"

struct AES_ctx ctx;

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

void decrypt(uint8_t* line) {
	// set the symmetric key
	uint8_t key[] = "YELLOW SUBMARINE";
	
	// initialize the AES cipher with the key
	AES_init_ctx(&ctx, key);
	
	// decrypt the line
	AES_ECB_decrypt(&ctx, line);
}

void print_dec_block(unsigned char* out) {
	// set block size of 128 bits (16 chars)
	uint8_t decr_blk[16];
	// set block index
	int blk_i = 0;
	// copy contents of out and decrypt every 128 bits
	for (int i = 0; i < 2880; i++) {
		// begin block decryption
		if (blk_i == 16) {
			// reset block index
			blk_i = 0;
			// null terminate string
			decr_blk[16] = 0;
			// decrypt block
			decrypt(decr_blk);
			printf("%s", decr_blk);
		}
		// copy buffer contents
		decr_blk[blk_i] = out[i];
		// increment block index
		blk_i++;
	}
	// decrypt and print the final block
	decr_blk[16] = 0;
	decrypt(decr_blk);
	printf("%s", decr_blk);
}

int main() {
	// open the file
	FILE* fp = fopen("7.txt", "r");
	// save space for file contents
	char* arr = malloc(3904);
	// save space for buffer
	char* buffer = malloc(62);
	// save space for base64 output
	unsigned char* out = malloc(5120);
	
	// extract each base64 line from the file
	while(fgets(buffer, 60, fp)) {
		// concatenate buffer
		strcat(arr, buffer);
		// remove unnecessary newline
		strtok(arr, "\n");
	}

	size_t sz = (size_t)5120;

	// decode base64 string
	Base64Decode(arr, &out, &sz);

	// begin sequential decryption
	// FIXME: try to decrypt in parallel
	print_dec_block(out);

	// free all allocated memory
	free(buffer);
	free(out);
	free(arr);
	return 0;
}
