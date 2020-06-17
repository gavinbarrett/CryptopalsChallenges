#include <stdio.h>
#include <openssl/bn.h>

/*
void egcd(BIGNUM *k, BIGNUM *prime, BIGNUM *x, BIGNUM *y) {
	if (BN_is_zero(k))
		x = BN_zero();
		y = BN_one();
		return;
	egcd(mod(prime, k), k, x, y);
	BIGNUM *g = BN_copy(prime);
	BIGNUM *x1 = BN_copy(x);
	BIGNUM *y1 = BN_copy(y);
	
	// y = x
	// x = y - (floor(prime/k)) * x
}

void mod_inverse(BIGNUM *k, BIGNUM *prime) {
	
}
*/

int main() {
	
	BIGNUM *p, *q, *n, *et, *e, *d, *p1, *q1, *one, *input, *c, *m;
	BN_CTX *ctx;
	ctx = BN_CTX_new();
	char* c_str;
 	
	p = BN_new();
 	q = BN_new();   
 	n = BN_new();
	et = BN_new();
	e = BN_new();
	d = BN_new();
	p1 = BN_new();
	q1 = BN_new();
	one = BN_new();
	d = BN_new();
	input = BN_new();
	c = BN_new();
	m = BN_new();

	BN_set_word(one, 1);
	BN_set_word(e, 3);
	BN_sub(p1, p, one);
	BN_sub(q1, q, one);

 	// generate two primes, a and b
	BN_generate_prime_ex(p, 512, 1, NULL, NULL, NULL);
	BN_generate_prime_ex(q, 512, 1, NULL, NULL, NULL);

	BN_mul(n, p, q, ctx);
	BN_mul(et, p1, q1, ctx);

	c_str = BN_bn2dec(p);
	//printf("%s\n", c_str);
	c_str = BN_bn2dec(q);
	//printf("%s\n", c_str);

	BN_mod_inverse(d, e, et, ctx);

	const char* in = "68656c6c6f207468657265";
	BN_hex2bn(&input, in);
	c_str = BN_bn2dec(input);
	printf("\n%s\n", c_str);

	// pubkey: [e,n] privkey: [d,n]
	BN_mod_exp(c, input, e, n, ctx);
	// compute c = input**e%n
	// compute m = c**d%n

	printf("\nEncrypted:\n");
	c_str = BN_bn2dec(c);
	printf("%s\n", c_str);

	BN_mod_exp(m, c, d, n, ctx);

	printf("\nDecrypted:\n");
	c_str = BN_bn2dec(m);
	printf("%s\n", c_str);
	
	BN_CTX_free(ctx);
	return 0;
}
