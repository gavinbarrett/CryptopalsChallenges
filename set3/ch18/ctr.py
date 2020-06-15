from base64 import b64decode
from Crypto.Cipher import AES

def decrypt(keystream, ciphertext):
	plaintext = ""
	for idx, i in enumerate(ciphertext):
		plaintext += chr(keystream[idx] ^ i)
	return plaintext


key = "YELLOW SUBMARINE"

nonce = 0

encoded_ciphertext = "L77na/nrFsKvynd6HzOoG7GHTLXsTVu9qvY/2syLXzhPweyyMTJULu/6/kXX0KSvoOLSFQ=="

ciphertext = b64decode(encoded_ciphertext)

cipher = AES.new(key, AES.MODE_ECB)

plaintext = ""
for idx, i in enumerate(range(0, len(ciphertext), 16)):
	c = ciphertext[idx:idx+16]
	# turn nonce into 16 bit number
	n = (nonce).to_bytes(16, byteorder="little")
	l = len(n)/2
	be = n[int(l):]
	n = be + n[:int(l)]
	n = n.decode('ascii')
	# increment nonce
	nonce += 1
	# generate keystream

	keystr = cipher.encrypt(n)
	#print(keystr)
	plaintext += decrypt(keystr, c)
	
print(plaintext)
