from base64 import b64decode
from Crypto.Cipher import AES

def decrypt(keystream, ciphertext):
	length = len(ciphertext)
	keystream = keystream[:length]
	plaintext = int(keystream.hex(),16) ^ int(ciphertext.hex(),16)
	return bytes.fromhex("{0:0{1}x}".format(plaintext,32))

key = "YELLOW SUBMARINE"

nonce = 0

encoded_ciphertext = "L77na/nrFsKvynd6HzOoG7GHTLXsTVu9qvY/2syLXzhPweyyMTJULu/6/kXX0KSvoOLSFQ=="

ciphertext = b64decode(encoded_ciphertext)
cipher = AES.new(key, AES.MODE_ECB)

plaintext = ""
for i in range(0, len(ciphertext), 16):
	
	c = ciphertext[i:i+16]
	
	# turn nonce into 16 bit number
	n = b'\x00\x00\x00\x00\x00\x00\x00\x00' + (nonce).to_bytes(8, byteorder="little")
	# increment nonce
	nonce += 1

	# generate keystream
	keystr = cipher.encrypt(n)
	
	print(decrypt(keystr, c).decode(), end="")
