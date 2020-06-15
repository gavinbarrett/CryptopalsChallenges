from Crypto.Cipher import AES
from binascii import unhexlify

key = "YELLOW SUBMARINE"
cipher = AES.new(key, AES.MODE_ECB)

def decipher(block):
	return cipher.decrypt(block)

def xor_block(block, ciphertext):
	e = []
	for i in range(len(block)):
		e += [ord(block[i]) ^ ciphertext[i]]
	return bytes(e)

def main():
	text = []
	f = open("10.txt","r")
	for i in f:
		text += i[:-1]
	text = ''.join(text)
	iv = [0x00] * 16
	for i in range(0,len(text),16):
		block = text[i:i+16]
		x_block = xor_block(block, iv)
		enc_block = decipher(x_block)
		print(enc_block)
		iv = [x for x in enc_block] 
main()
