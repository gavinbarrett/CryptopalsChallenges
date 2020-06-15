#include <stdio.h>
#include <string.h>
#include <time.h>
#include <gmp.h>
#include <openssl/sha.h>

int main(void) {
	unsigned long seed = time(NULL);
	mpz_t p;
	// initialize mpz objects for random numbers
	mpz_t a;
	mpz_t A;
	mpz_t b;
	mpz_t B;
	mpz_t s_key_1;
	mpz_t s_key_2;
	mpz_init(a);
	mpz_init(A);
	mpz_init(b);
	mpz_init(B);
	mpz_init(s_key_1);
	mpz_init(s_key_2);
	mpz_init_set_str(p, "ffffffffffffffffc90fdaa22168c234c4c6628b80dc1cd129024e088a67cc74020bbea63b139b22514a08798e3404ddef9519b3cd3a431b302b0a6df25f14374fe1356d6d51c245e485b576625e7ec6f44c42e9a637ed6b0bff5cb6f406b7edee386bfb5a899fa5ae9f24117c4b1fe649286651ece45b3dc2007cb8a163bf0598da48361c55d39a69163fa8fd24cf5f83655d23dca3ad961c62f356208552bb9ed529077096966d670c354e4abc9804f1746c08ca237327ffffffffffffffff", 16);
	mpz_t g;
	mpz_init_set_ui(g, 2);
	
	// initialize GMP's random number state
	gmp_randstate_t r_state;
	// initialize GMP's random number generator with a Mersenne Twister
	gmp_randinit_default(r_state);
	// get a seed for the generator
	gmp_randseed_ui(r_state, seed);
	
	// compute random a
	mpz_urandomb(a, r_state, 80);
	mpz_mod(a, a, p);
	// compute random b	
	mpz_urandomb(b, r_state, 80);
	mpz_mod(b, b, p);

	// compute A = g**a % p
	mpz_powm(A, g, a, p);
	//modular_expo(g, a, p, &A);
	// compute B = g**b % p
	mpz_powm(B, g, b, p);
	//modular_expo(g, b, p, &B);

	// compute session key 1 = B**a % p
	mpz_powm(s_key_1, B, a, p);
	//modular_expo(B, a, p, &s_key_1);
	// compute session key 2 = A**b % p
	mpz_powm(s_key_2, A, b, p);
	//modular_expo(A, b, p, &s_key_2);

	//gmp_printf("DH Key 1: %Zd\n", s_key_1);
	//gmp_printf("DH Key 2: %Zd\n", s_key_2);
	
	char str[2000];
	mpz_get_str(str, 10, s_key_1);

	unsigned char* d = SHA256((unsigned char*)str, strlen(str), 0);
	for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
		printf("%02x", d[i]);
	return 0;
}
