#include <stdio.h>
#include <string.h>

void encrypt(char string[], char key[], int index) {
	for (int i = 0; i < (int)strlen(string); i++) {
		string[i] ^= key[index];
		index = (index+1) % 3;
	}
}

void print_hex(char hex[]) {
	for (int i = 0; i < (unsigned int)strlen(hex); i++)
		printf("%02x", hex[i]);
	printf("\n");
}

int main() {

	char key[4] = "ICE";
	
	static int index = 0;

	char str1[76] = "Burning 'em, if you ain't quick and nimble\nI go crazy when I hear a cymbal";

	encrypt(str1, key, index);
	print_hex(str1);
	//encrypt(str2, key, index);
	//print_hex(str2);

	return 0;
}
