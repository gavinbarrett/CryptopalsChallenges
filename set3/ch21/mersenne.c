#include <stdio.h>
#include <stdlib.h>

unsigned int a = 0x9908B0DF;
unsigned int u = 11;
unsigned int m = 397;
unsigned int t = 15;
unsigned int s = 7;
unsigned int b = 0x9D2C5680;
unsigned int c = 0xEFC60000;
unsigned int l = 18;
unsigned int f = 1812433253;
unsigned int n = 624;
unsigned int w = 32;

unsigned int MT[623];
unsigned int index = 624 + 1;

unsigned int lower_mask = (1L << 31) - 1;
unsigned int upper_mask = ~((1L << 31) - 1);

void seed_mt(unsigned int seed) {
	index = n;
	MT[0] = seed;
	for (int i = 1; i < n-1; i++)
		MT[i] = (f * (MT[i-1] ^ (MT[i-1] >> (w-2))) + i);
}

void twist() {
	for (int i = 0; i < (n-1); i++) {
		unsigned int x = (MT[i] & upper_mask) + (MT[(i + 1) % n] & lower_mask);
		unsigned int xA = x >> 1;
		if ((x % 2) != 0)
			xA = xA ^ a;
		MT[i] = MT[(i + m) % n] ^ xA;
	}
	index = 0;
}

unsigned int extract_number() {
	if (index >= n) {
		if (index > n) {
			printf("Generator was never seeded\n");
			exit(0);
		}
		twist();
	}
	unsigned int y = MT[index];
	y ^= (y >> u);
	y ^= ((y << s) & b);
	y ^= ((y << t) & c);
	y ^= (y >> l);
	index++;
	return y;
}

int main() {
	unsigned int s = 1131464071;
	seed_mt(s);
	for (int i = 0; i < 10; i++)
		printf("%u\n", extract_number());
	return 0;
}
