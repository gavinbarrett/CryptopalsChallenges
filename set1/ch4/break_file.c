#include <stdio.h>
#include <string.h>
#include "single_byte_xor.h"

int main(void) {

	FILE* fp = fopen("file.txt", "r");
	char buffer[62];
	char plaintext[62] = {0};
	char key;
	while (fgets(buffer, 62, fp) != NULL) {
		//printf("line: %s\n", buffer);
		key = break_cipher(buffer, plaintext);
		printf("Key: %d\nPlaintext: %s\n", key, plaintext);
	}

	return 0;
}
