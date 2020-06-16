from base64 import b64decode
from Crypto.Cipher import AES

key = "YELLOW SUBMARINE"
cipher = AES.new(key, AES.MODE_ECB)

def decipher(block):
	return cipher.decrypt(block)

def xor_block(block, ct):
	x = int(block.hex(), 16) ^ int(ct.hex(), 16)
	return bytes.fromhex("{0:0{1}x}".format(x,32))

def main():
	text = []
	f = open("10.txt","r")
	for i in f:
		text += i
	text = b64decode(''.join(text))
	iv = b'\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'
	for i in range(0,len(text),16):
		block = text[i:i+16]
		nblock = decipher(block)
		x_block = xor_block(nblock, iv)
		iv = block
		print(f"{x_block.decode()}", end="")
main()
