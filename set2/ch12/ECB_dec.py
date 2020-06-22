import string
from os import urandom
from base64 import b64decode
from Crypto.Cipher import AES

# generate a random key for AES encryption
key = urandom(16)
# initialize the cipher
cipher = AES.new(key, AES.MODE_ECB)
# initialize our secret message to be collected
secret = "Um9sbGluJyBpbiBteSA1LjAKV2l0aCBteSByYWctdG9wIGRvd24gc28gbXkgaGFpciBjYW4gYmxvdwpUaGUgZ2lybGllcyBvbiBzdGFuZGJ5IHdhdmluZyBqdXN0IHRvIHNheSBoaQpEaWQgeW91IHN0b3A/IE5vLCBJIGp1c3QgZHJvdmUgYnkK"

def encrypt_buffer(st, j):
	''' Encrypt a 16-byte buffer under AES-ECB '''
	# append the input string to the secret string
	string = st.encode() + b64decode(secret)[j:]
	# return the ciphertext of the first block
	return cipher.encrypt(''.join([string[0:16].decode()]))

def find_block_size():
	''' Find the encryption function's block size '''
	past_ctext = 0
	# check block size from 1 to 64
	for i in range(1, 128):
		# create a buffer of i As
		As = 'A' * i
		# get ciphertext of the buffer
		ctext = encrypt_buffer(As, 0)
		# if ciphertexts match, we've found the block size
		if (past_ctext == ctext):
			return i-1
		past_ctext = ctext

def attack_ecb(blk_sz):
	alphabet = string.printable
	m_len = len(b64decode(secret))
	block = 'A' * (blk_sz-1)
	plaintext = ""
	for i in range(m_len):
		enc = encrypt_buffer(block, i)
		for char in alphabet:
			n_block = block + char	
			n_enc = encrypt_buffer(n_block, i)
			if (enc == n_enc):
				block = n_block[1:]
				break
		plaintext += char
	plaintext += char
	return plaintext

def decrypt_secret():
	# find the block size
	blk_sz = find_block_size()
	print(f"Block size: {blk_sz}\n")
	# make a buffer of all As
	blk1 = 'A' * blk_sz

	# make two calls to the encryption function
	# if ECB is being used, ciphertexts will be equal
	enc1 = encrypt_buffer(blk1, 0)
	enc2 = encrypt_buffer(blk1, 0)

	# check if ECB is being used
	if (enc1 == enc2):
		print(f"ECB mode detected!\n")
		plaintext = attack_ecb(blk_sz)
		print(f"Decrypted plaintext:\n\n{plaintext}")

decrypt_secret()
