def mod_exp(base, exp, mod):
	if mod == 1:
		return 0
	c = 1
	for e_prime in range(exp):
		c = c*base % mod
	return c

print(mod_exp(234,23, 47))
