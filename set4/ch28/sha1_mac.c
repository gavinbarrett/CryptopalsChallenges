#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>

int main() {
	
	unsigned char data[] = "This is a message\0";
	unsigned char hash[SHA_DIGEST_LENGTH];
	int len = (int)strlen((char*)data);

	SHA1(data, len, hash);
	
	for (int i = 0; i < (int)SHA_DIGEST_LENGTH; i++)
		printf("%02x", hash[i]);

	return 0;
}
