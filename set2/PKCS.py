BLOCK_SZ = 20

def pad(x):
	off = BLOCK_SZ - (len(x) % BLOCK_SZ)
	padding = off * '\x04'
	return x + padding

def main():
	x = "YELLOW SUBMARINE"
	print(pad(x))
	for i in pad(x):
		print(ord(i))

main()
