import os
from hashlib import sha256
from base64 import b64encode
from binascii import unhexlify

def mod_exp(base, exp, mod):
	if mod == 1:
		return 0
	c = 1
	for e_prime in range(exp):
		c = c*base % mod
	return c

password = b"secretp@ss"
prime = "73374887456294034884101742758304236415021425545608561364843267496387553962670503193922662042567517060777660670203359981229527925590585527244774428396301338C18E5DC98684E2A15B84535635A95C4A192B73B40A780AB4CB0C58BDB9C31EF970C3AC6D804712B830FB6F1B140693A251E989F89B687EBA62781AD031D5135"
prime = int(prime, 16)
salt = b64encode(os.urandom(16))
#print(salt)
xH = sha256(salt + password)
i = int(xH.digest().hex(),16)
#print(i)
v = mod_exp(2,i,prime)
print(v)
