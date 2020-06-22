from base64 import b64decode
from collections import defaultdict

alphabet = ['a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',' ']

freq_table = [8.12,1.49,2.71,4.32,12.02,2.30,2.03,5.92,7.31,0.10,0.69,3.98,2.61,6.95,7.68,1.82,0.11,6.02,6.28,9.10,2.88,1.11,2.09,0.17,2.11,0.07,18.29]

def hti(h):
	if ((h > 96) and (h < 103)):
		return h - 87
	return h - 48

def decode(a, b):
	a = hti(ord(a))
	b = hti(ord(b))
	a = a << 4
	return a | b

def compute_exp(c, n_obs, l):
	if ((c > 96) and (c < 123)):
		c -= 97
		return freq_table[c] * l
	elif ((c > 64) and (c < 91)):
		c -= 65
		return freq_table[c] * l
	elif (c == 32):
		c -= 6
		return freq_table[c]
	else:
		return n_obs

def compute_obs(c, string):
	sz = len(string)
	acc = 0.0
	for i in string:
		if (i == c):
			acc+=1
	return acc / sz

def score_code(newcode):
	sz = len(newcode)
	acc = 0.0
	for i in newcode:
		n_obs = compute_obs(i, newcode)
		n_exp = compute_exp(ord(i), n_obs, len(newcode))
		if (n_exp == 0):
			continue
		n = (n_obs - n_exp)
		n2 = n*n
		res = (n2 / n_exp)
		acc += res
	return acc

def break_cipher(code):
	sz = len(alphabet)
	clen = len(code)-1
	score = 0.0
	sc = 0.0
	plaintext = ''
	key = 0
	#b = defaultdict(lambda: 0.0)
	for i in range(0, sz):
		newcode = []
		for j in range(clen):
			newcode += chr(code[j] ^ ord(alphabet[i]))
		sc = score_code(newcode)
		if sc > score:
			#b[''.join(newcode)] = score
			score = sc
			plaintext = newcode
			key = alphabet[i]
	return plaintext, key

def pop_count(c):
	count = 0
	while (c > 0):
		if (c & 0x01) == 1:
			count += 1
		c >>= 1
	return count

def normalized_hamming(str1, str2):
	count = 0
	for i in range(len(str1)):
		count += pop_count(str1[i] ^ str2[i])
	return (count * 1.0)/len(str1)

def encrypt(string, key):
	new_string = []
	k_length = len(key)
	index = 0
	for i in range(len(string)):
		if isinstance(key[index], int):
			new_string.append(string[i] ^ key[index])
		else:
			new_string.append(string[i] ^ ord(key[index]))
		index = (index+1) % k_length
	return ''.join(chr(n) for n in new_string)

def solve_blocks(blocks):
	keys = []
	for block in blocks:
		plaintext, key = break_cipher(block)
		keys += [key]
	return ''.join(keys)

def solve_hamming(base, size):
	bs = [base[i:i+size] for i in range(0,len(base),size)]
	if not all(len(b) == size for b in bs):
		bs = bs[:-1]
	ham = 0
	for i in range(2,len(bs)-1):
		str1, str2 = bs[i], bs[i+1]
		ham +=  (normalized_hamming(str1,str2)/i)
	#str1 = base[:size]
	#str2 = base[size:size+size]
	#ham = hamming(str1, str2)/size
	return ham/len(bs)

def get_file_contents():
	f = open('6.txt','r')
	l = ""
	for line in f:
		l += line[:-1]
	f.close()
	return b64decode(l)

def get_key_sizes(base):
	inf = float('inf')
	key_sizes = []
	for i in range(2,40):
		h = solve_hamming(base, i)
		if (h < inf):
			inf = h
			key = i
			key_sizes += [i]
	return key_sizes

def decrypt(base, repeat_key):
	a = 0
	for i in base:
		c = bytes([i ^ ord(repeat_key[a])])
		a = (a + 1) % len(repeat_key)
		print(c.decode(), end='')

def main():
	# decode file contents
	base = get_file_contents()
	
	# find the best key sizes
	key_sizes = get_key_sizes(base)
	
	# try each key
	for k in key_sizes:
		
		# split the ciphertexts into size k blocks
		cipherblocks = [list(base[j:j+k]) for j in range(0,len(base),k)]
		
		# remove the last block if it is less than k in length
		if not all(len(c) == k for c in cipherblocks):
			cipherblocks = cipherblocks[:-1]
		
		# transpose the blocks
		transposed_blocks = list(zip(*cipherblocks))
		
		# solve each block as a single byte xor
		repeat_key = solve_blocks(transposed_blocks)

		# decrypt the ciphertext with the key	
		decrypt(base, repeat_key)
	
main()
