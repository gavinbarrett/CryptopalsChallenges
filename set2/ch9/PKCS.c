#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BLOCK_SZ 16

void pad(char* string) {
	//char pad = 0x04;
	int len = (int)strlen(string);
	int off = (len % BLOCK_SZ);
	if (off > 0)
		off = BLOCK_SZ - off;
	//string = realloc(string, len+off);
	//char* ps = malloc(off);
	int i = len;
	printf("len: %d\noff: %d\n", len, off);
	for (; i < len+off; i++)
		string[i] = off;
		//ps[i] = off;
	//ps[i] = 0;

	//strcat(string, ps);
	//free(ps);
}

int main() {
	char x[1000] = {0};
	strcpy(x, "YELLOW 9");
	pad(x);
	for (int i = 0; i < (int)strlen(x); i++)
		printf("%02x ", x[i]);
	return 0;
}
