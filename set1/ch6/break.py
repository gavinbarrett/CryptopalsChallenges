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
	#print(c)
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
		#print(f"newcode[i]: {newcode[i]}")
		n_obs = compute_obs(i, newcode)
		#print(f"n_obs: {n_obs}")
		n_exp = compute_exp(ord(i), n_obs, len(newcode))
		#print(f"n_exp: {n_exp}")
		#print(f"n_obs: {n_obs}, n_exp: {n_exp}")
		if (n_exp == 0):
			continue
		n = (n_obs - n_exp)
		#print(f"n: {n}")
		
		n2 = n*n
		#print(f"n2: {n2}")
		
		#print(f"n: {n}")
		res = (n2 / n_exp)
		#print(f"res: {res}")
		
		#print(res)
		acc += res
	#print(f"acc: {acc}")
	return acc

def break_cipher(code):
	sz = len(alphabet)
	clen = len(code)-1
	score = 0.0
	sc = 0.0
	plaintext = ''
	b = defaultdict(lambda: 0.0)
	for i in range(0, sz):
		newcode = []
		for j in range(0, clen, 2):
			newcode += chr(ord(code[j]) ^ ord(alphabet[i]))
		sc = score_code(newcode)
		if sc > score:
			b[''.join(newcode)] = score
			score = sc
			plaintext = newcode
			key = alphabet[i]
	dd = sorted(b.items(), key=lambda x:x[1])
	return plaintext, key

def pop_count(c):
	count = 0
	while (c > 0):
		if (c & 0x01) == 1:
			count += 1
		c >>= 1
	return count

def hamming(str1, str2):
	count = 0
	for i in range(len(str1)):
		count += pop_count(str1[i] ^ str2[i])
	return count * 1.0

def encrypt(string, key):
	new_string = []
	k_length = len(key)
	index = 0
	for i in range(len(string)):
		new_string.append(string[i] ^ ord(key[index]))
		index = (index + 1) % k_length
	return ''.join(chr(n) for n in new_string)

def main():
	f = open('6.txt','r')
	l = ""
	inf = float('inf')
	for line in f:
		l += line[:-1]
	base = b64decode(l)
	#print(base)
	for i in range(2,40):
		str1 = base[:i]
		str2 = base[i:i+i]
		h = (hamming(str1, str2)*1.0)/i
		if (h < inf):
			inf = h
			key = i
	blocks = []
	for i in range(key):
		blocks.append(list())
	for i in range(0, (len(base)-key),key):
		for j in range(key):
			blocks[j] += [base[i+j]]
	'''
	for b in blocks:
		print(b)
	'''
	print(blocks)	
	
	#bs = []
	#for i in range(len(blocks)):
		#bs.append(list())
	#for idx in range(len(blocks[0])):
	#	c = [x[idx] for x in blocks]
	#	for i, v in enumerate(c):
	#		bs[i] += [chr(v)]
	
	#bs = [''.join(x) for x in bs]
	#print(bs)
	k = []	
	
	for e in blocks:
		e = ''.join([chr(b) for b in e])
		plaintext, key = break_cipher(e)
		k += key
	k = ''.join(k)
	print(k)
	f.close()
	
	print(encrypt(base, k))

main()
