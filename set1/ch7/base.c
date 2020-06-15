#include <stdio.h>
#include <string.h>

char b64decode(char c) {
	if ((c > 64) && (c < 91))
		return c - 65;
	else if ((c > 96) && (c < 123))
		return c - 71;
	else if ((c > 47) && (c < 58))
		return c + 4;
	else if (c == '+')
		return c + 19;
	else if (c == '/')
		return c + 16;
	return 0;
}

void printer(unsigned char cs[]) {
	unsigned char c = 0;
	int b = 0x00000000;
	int s = 3;
	for (int i = 0; i < 4; i++) {
		c = cs[i] << 2;
		b |= (c << (6*s));
		s--;
		//printf("%d ", cs[i]);
	}
	for (int j = 3; j > 0; j--) {
		char c = (b >> (6*(j-1)));
		printf("%c ", c);
	}
	//printf("%02X", b);
	printf("\n");
}

int main() {

	FILE* fp = fopen("7.txt", "r");
	char buffer[62];
	unsigned char cs[4];
	int x = 0;
	while(fgets(buffer, 61, fp)) {
		for (int i = 0; i < 61; i++) {
			if (x == 4) {
				printer(cs);				
				x = 0;
			}
			if (buffer[i] == '\n')
				continue;
			cs[x] = b64decode(buffer[i]);
			x++;
		}
	}

	return 0;
}
