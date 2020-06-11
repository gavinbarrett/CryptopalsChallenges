#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

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

int get_random(int min, int max) {
	return min + rand() % (max + 1 - min);
}

int get_timestamp() {
	return (int)time(NULL);
}

int crack_seed() {
	// get current time
	int start_time = get_timestamp();
	printf("Starting analysis at %d\n", start_time);

	// seed random generator for wait_time
	srand(start_time);
	
	// get a random wait time
	int wait_time = get_random(40, 1000);

	// wait random number of second
	sleep(wait_time);

	// seed the MT generator with the time after waiting
	seed_mt(get_timestamp());
	
	// save the number for the seed we want to crack
	int number = extract_number();

	// 
	for (int i = 40; i < 1000; i++) {
		seed_mt(start_time + i);
		if (number == extract_number()) {
			return start_time + i;
		}
	}
	return 0;
}

int main() {
	int seed = crack_seed();
	if (seed)
		printf("Seed cracked!\n%d\n", seed);
	return 0;
}
