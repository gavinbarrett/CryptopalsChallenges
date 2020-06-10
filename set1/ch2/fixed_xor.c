#include <stdio.h>

unsigned char hex_table[16] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};

int hex_to_int(unsigned char hex) {
	if ((hex > 96) && (hex < 103))
		return hex - 87;
	else
		return hex - 48;
}

int main() {
	int sz = 36;
	char* buff1 = "1c0111001f010100061a024b53535009181c";
	char* buff2 = "686974207468652062756c6c277320657965";

	for (int i = 0; i < sz; i++)
		printf("%c", hex_table[hex_to_int(buff1[i]) ^ hex_to_int(buff2[i])]);
	
	printf("\n");

	return 0;
}
