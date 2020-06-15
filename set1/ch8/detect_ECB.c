#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {

	FILE* fp = fopen("8.txt","r");
	
	char* buffer = malloc(322);
	int l = 0;
	int max_cnt = 0;
	int line = 0;
	while(fgets(buffer, 322, fp)) {
		strtok(buffer, "\n");
		char blocks[100][100];
		int k = 0;
		for (int i = 0; i < 320; i+=32) {
			strncpy(blocks[k], &buffer[i], 32);
			k++;
		}
		int line_cnt = 0;
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				if (strcmp(blocks[i], blocks[j]) == 0)
					line_cnt++;
			}
		}
		if (line_cnt > max_cnt) {
			line = l;
			max_cnt = line_cnt;
		}
		l++;
	}
	free(buffer);

	printf("Detecting ECB mode on line %d\n", line);

	return 0;
}
