#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BLOCK_SZ 20

void pad(char* string) {
	char pad = 0x04;
	int len = (int)strlen(string);
	int off = BLOCK_SZ - (len % BLOCK_SZ) + 1;
	string = realloc(string, len+off);
	char* ps = malloc(off);
	int i = 0;
	for (; i < off-1; i++)
		ps[i] = pad;
	ps[i] = 0;

	strcat(string, ps);
	free(ps);
}

int main() {
	char* x = malloc(33);
	strcpy(x, "YELLOW SUBMARINEYELLOW SUBMARINE");
	pad(x);
	for (int i = 0; i < (int)strlen(x); i++)
		printf("%02x\n", x[i]);
	free(x);
	return 0;
}
