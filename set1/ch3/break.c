#include <stdio.h>
#include "single_byte_xor.h"

int main(void) {
	char code[69] = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
	char plaintext[35] = {0};
	char key = break_cipher(code, plaintext);
	if (key == 'X') {
		printf("Cipher broken!\n");
		printf("\nPlaintext:\n%s\n", plaintext);
	}
	return 0;
}
