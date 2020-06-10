#include <stdio.h>

int hex_to_int(unsigned char hex) {
	if ((hex > 96) && (hex < 103))
		return hex - 87;
	else
		return hex - 48;
}

unsigned char base_table[64] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};

int main() {
	int sz = 95;
	unsigned char str[96] = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
	
	unsigned char f = 0;
	unsigned char s = 0;

	for (int i = 0; i <= sz; i += 3) {
		f = 0;
		s = 0;
		// copy entire first char
		f |= hex_to_int(str[i]) << 2;
		// copy half of the second char
		f |= hex_to_int(str[i+1]) >> 2;
		// print number
		printf("%c", base_table[f]);
		// next half of char
		s |= ((hex_to_int(str[i+1]) & 3) << 4);
		// copy third char
		s |= hex_to_int(str[i+2]);
		// print number
		printf("%c", base_table[s]);
	}
	printf("\n");
	return 0;
}
