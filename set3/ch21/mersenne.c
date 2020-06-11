#include <stdio.h>
#include <stdlib.h>

int a = 0x9908B0DF;
int u = 11;
int m = 397;
long d = 0xFFFFFFFF;
int t = 15;
int s = 7;
long b = 0x9D2C5680;
long c = 0xEFC60000;
int l = 18;
int f = 1812433253;
int n = 624;
int w = 32;
int MT[623];
int index = 624 + 1;

unsigned int lower_mask = (1L << 31) - 1;
unsigned int upper_mask = ~((1L << 31) - 1);

void seed_mt(int seed) {
	index = n;
	MT[0] = seed;
	for (int i = 1; i < n; i++) {
		MT[i] = (f * (MT[i-1] ^ (MT[i-1] >> (w-2))) + i);
	}
}

void twist() {
	for (int i = 0; i < (n); i++) {
		int x = (MT[i] & upper_mask) + (MT[(i + 1) % n] & lower_mask);
		int xA = x >> 1;
		if ((x % 2) != 0) {
			xA = xA ^ a;
		}
		MT[i] = MT[(i + m) % n] ^ xA;
	}
	index = 0;
}

int extract_number() {
	if (index >= n) {
		if (index > n) {
			printf("Generator was never seeded\n");
			exit(0);
		}
		twist();
	}
	int y = MT[index];
	y = y ^ ((y >> u) & d);
	y = y ^ ((y << s) & b);
	y = y ^ ((y << t) & c);
	y = y ^ (y >> l);

	index++;
	return y;
}

int main() {
	int s = 1131464071;
	seed_mt(s);
	printf("%d\n", extract_number());
	return 0;
}
