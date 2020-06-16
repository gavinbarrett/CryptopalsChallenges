#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define BLOCK_SZ 16

/*
int check_validity(char str[]) {
	char newstr[16];
	int end = 0;
	for (int i = 0; i < (int)strlen(str); i++) {
		if (isprint(str[i]) && !end) {
			newstr[i] = str[i];
		} else {
			end = 1;
			if (str[i] == 0x04)
				continue;
			else
				return 0;
		}
	}
	strcpy(newstr, str);
	return 1;
}
*/

int check_validity(char str[]) {
	int sz = (int)strlen(str);
	char pad_byte = str[sz-1];
	if (pad_byte > 0xff)
		return 1;
	for (int i = sz-1; i > sz-((int)pad_byte)-1; i--) {
		if (str[i] == pad_byte)
			continue;
		else
			return 0;
	}
	return 1;
}

void print_out(char str[]) {
	for (int i = 0; i < (int)strlen(str); i++)
		printf("%c", str[i]);
}

int main() {

	char valid[] = "ICE ICE BABY\x04\x04\x04\x04";
	char invalid1[] = "ICE ICE BABY\x05\x05\x05\x05";
	char invalid2[] = "ICE ICE BABY\x01\x02\x03\x04";
	
	if (check_validity(valid)) {
		printf("Valid\n");
		print_out(valid);
	} 
	if (check_validity(invalid1)) {
		printf("Invalid1\n");
		print_out(invalid1);
	}
	if (check_validity(invalid2)) {
		printf("Invalid2\n");
		print_out(invalid2);
	}
	return 0;
}
