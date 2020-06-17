import string
from os import urandom
from base64 import b64decode
from Crypto.Cipher import AES

key = urandom(16)
secret = "Um9sbGluJyBpbiBteSA1LjAKV2l0aCBteSByYWctdG9wIGRvd24gc28gbXkgaGFpciBjYW4gYmxvdwpUaGUgZ2lybGllcyBvbiBzdGFuZGJ5IHdhdmluZyBqdXN0IHRvIHNheSBoaQpEaWQgeW91IHN0b3A/IE5vLCBJIGp1c3QgZHJvdmUgYnkK"


def run(st, j):
	string = st.encode() + b64decode(secret)[j:]
	cipher = AES.new(key, AES.MODE_ECB)
	texts = []
	for i in range(0, 1):#len(string)-16, 16):
		s = ''.join([string[i:i+16].decode()])
		return cipher.encrypt(s)

m_len = len(b64decode(secret))
alphabet = string.printable
BLK_SZ = 16
block = 'A' * (BLK_SZ-1)
plain = ""
for i in range(m_len):
	enc = run(block, i)
	for a in alphabet:
		c = block + a	
		n_enc = run(c, i)
		if (enc == n_enc):
			block = c[1:]
			break
	plain += a
plain += a
print(f"Decrypted Plaintext:\n {plain}")
