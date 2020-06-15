#include <stdio.h>
#include <time.h>
#include <gmp.h>

void modular_expo(mpz_t base, mpz_t exp, mpz_t mod, mpz_t* out) {
	mpz_t z;
	mpz_init_set_ui(z, 0);
	if (mpz_cmp_ui(mod, 1) == 0)
		mpz_set((*out), z);
	mpz_set_ui((*out), 1);
	printf("Entering loop\n");
	gmp_printf("Exp: %Zd\n", exp);
	while(mpz_cmp(z, exp) != 0) {
		// out = out * base
		mpz_mul((*out), (*out), base);
		// out = out % mod
		mpz_mod((*out), (*out), mod);
		// z++
		mpz_add_ui(z, z, 1);
		//gmp_printf("z: %Zd\n", z);
	}
	printf("Exiting loop\n");
}

int main(void) {
	unsigned long seed = time(NULL);
	mpz_t p;
	// initialize mpz objects for random numbers
	//mpz_t rand;
	mpz_t a;
	mpz_t A;
	mpz_t b;
	mpz_t B;
	mpz_t s_key_1;
	mpz_t s_key_2;
	//mpz_init(rand);
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
	modular_expo(g, a, p, &A);
	// compute B = g**b % p
	modular_expo(g, b, p, &B);

	// compute session key 1 = B**a % p
	modular_expo(B, a, p, &s_key_1);
	// compute session key 2 = A**b % p
	modular_expo(A, b, p, &s_key_2);

	gmp_printf("%Zd\n", s_key_1);
	gmp_printf("%Zd\n", s_key_2);

	return 0;
}
