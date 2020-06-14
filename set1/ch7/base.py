import string
from base64 import b64decode
from Crypto.Cipher import AES

base_alpha = list(string.ascii_uppercase + string.ascii_lowercase + string.digits + "+/")

key = "YELLOW SUBMARINE"

f = open("7.txt", "r")

e = [i[:-1] for i in f]
f.close()

e = ''.join(e)

x = b64decode(e)

cipher = AES.new(key, AES.MODE_ECB)
cs = cipher.decrypt(x)
print(cs.decode())
