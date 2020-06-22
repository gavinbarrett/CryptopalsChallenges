import random
from hashlib import sha256

def mod_exp(base, exp, mod):
	if mod == 1:
		return 0
	c = 1
	for e_prime in range(exp):
		c = c*base % mod
	return c

p = "ffffffffffffffffc90fdaa22168c234c4c6628b80dc1cd129024e088a67cc74020bbea63b139b22514a08798e3404ddef9519b3cd3a431b302b0a6df25f14374fe1356d6d51c245e485b576625e7ec6f44c42e9a637ed6b0bff5cb6f406b7edee386bfb5a899fa5ae9f24117c4b1fe649286651ece45b3dc2007cb8a163bf0598da48361c55d39a69163fa8fd24cf5f83655d23dca3ad961c62f356208552bb9ed529077096966d670c354e4abc9804f1746c08ca237327ffffffffffffffff"
p = int(p,16)
g = 2

# Alice computes a random private key
a = random.randint(40,2000) % p
# Alice computes a public key A = g**a % p
A = mod_exp(g,a,p)

# Bob computes a random private key
b = random.randint(40,2000) % p
# Bob computes a public key B = g**b % p
B = mod_exp(g,b,p)

# Alice computes her session key
s2 = mod_exp(A,b,p)
# Bob computes his session key
s1 = mod_exp(B,a,p)


key1 = sha256(str(s1).encode("ascii")).hexdigest()
key2 = sha256(str(s2).encode("ascii")).hexdigest()

print(f"Key1: {key1}\n\nKey2: {key2}")
