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

def compute_exp(c, n_obs):
	print(f"c: {c}")
	if ((c > 96) and (c < 123)):
		c -= 97
		return freq_table[c]
	elif ((c > 64) and (c < 91)):
		c -= 65
		return freq_table[c]
	elif (c == 32):
		c -=5
		return freq_table[c]
	else:
		return n_obs

def compute_obs(c, string):
	sz = len(string)
	acc = 0.0
	for i in range(0, sz):
		if (string[i] == c):
			acc+=1
	return acc / sz

def score_code(newcode):
	sz = len(newcode)
	acc = 0.0
	for i in range(0, sz):
		#print(f"newcode[i]: {newcode[i]}")
		n_obs = compute_obs(newcode[i], newcode)
		n_exp = compute_exp(ord(newcode[i]), n_obs)
		#print(f"n_obs: {n_obs}, n_exp: {n_exp}")
		n = (n_obs - n_exp)
		n2 = n*n
		#print(f"n: {n}")
		res = (n2 / n_exp)
		#print(res)
		acc += res
	return acc

def break_cipher(code):
	sz = len(alphabet)
	clen = len(code)-1
	score = 0.0
	sc = 0.0
	plaintext = ''
	#print(f"code: {code}")
	for i in range(0, sz):
		newcode = []
		for j in range(0, clen//2, 2):
			print(f"{code[j]}{code[j+1]}")
			d = decode(code[j], code[j+1])
			#print(f"d: {d}")
			newcode += str(d ^ ord(alphabet[i]))
		#print(f"newcode: {newcode}")
		sc = score_code(newcode)
		#print(sc)
		if sc > score:
			score = sc
			plaintext = newcode
	return plaintext

def main():
	f = open('file.txt','r')
	for line in f:
		decoded = break_cipher(line)
		score = score_code(decoded)
		print(f"line: {decoded} score: {score}")
	f.close()

main()
