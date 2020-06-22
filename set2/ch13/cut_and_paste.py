from collections import defaultdict

#text = "foo=bar&baz=qux&zap=zazzle"
text = "email=foo@bar.com&uid=10&role=user"

key = ""
val = ""
k = True
d = defaultdict()
for i in text:
	if i == '&':
		# insert in dict
		d[key] = val
		key = ''
		val = ''
		k = True
	elif i == '=':
		k = False
	elif k:
		key += i
	else:
		val += i
d[key] = val
print(d)
