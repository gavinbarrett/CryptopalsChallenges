from base64 import b64decode
from Crypto.Cipher import AES


def main():
	key = "YELLOW SUBMARINE"
	f = open("7.txt", "r")
	cipher = AES.new(key, AES.MODE_ECB)
	for i in f:
		c = b64decode(i)
		c += b'\x00\x00\x00'
		print(len(c))
		plaintext = cipher.decrypt(c)
		print(plaintext)

	f.close()
main()
