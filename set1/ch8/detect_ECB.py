from binascii import unhexlify

f = open('8.txt', 'r')

for line in f:
	print(str(bytes.fromhex(line)))

f.close()
