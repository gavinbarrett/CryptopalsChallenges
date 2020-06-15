import random
from hashlib import sha256
p = 37
g = 5

a = random.randint(40,2000) % p
A = g**a % p

b = random.randint(40,2000) % p
B = g**b % p

s1 = B**a % p
s2 = A**b % p

key = sha256(str(s1).encode("ascii")).hexdigest()
print(f"Key: {key}")
