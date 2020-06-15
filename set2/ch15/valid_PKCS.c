#include <stdio.h>
#include <string.h>
#include <ctype.h>

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

void print_out(char str[]) {
	for (int i = 0; i < (int)strlen(str); i++)
		printf("%c", str[i]);
}

int main() {

	char valid[] = "ICE ICE BABY\x04\x04\x04\x04";
	char invalid1[] = "ICE ICE BABY\x05\x05\x05\x05";
	char invalid2[] = "ICE ICE BABY\x01\x02\x03\x04";
	
	if (check_validity(valid))
		print_out(valid);
	if (check_validity(invalid1))
		print_out(invalid1);
	if (check_validity(invalid2))
		print_out(invalid2);

	return 0;
}
