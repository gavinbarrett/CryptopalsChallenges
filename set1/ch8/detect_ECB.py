from binascii import unhexlify
from collections import defaultdict

f = open('8.txt', 'r')

d = defaultdict(lambda: 0)

for idx, line in enumerate(f):
	l = bytes.fromhex(line)
	blocks = [l[i:i+16] for i in range(0,len(l),16)]
	line_cnt = 0
	for b in blocks:
		for e in blocks:
			if (b == e):
				line_cnt += 1
	d[idx] = line_cnt
s = sorted(d.items(), key=lambda x:x[1], reverse=True)[0]
print(f"Detecting ECB mode on line {s[0]}")
f.close()
