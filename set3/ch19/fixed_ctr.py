from os import urandom
from base64 import b64decode
from binascii import hexlify
from Crypto.Cipher import AES

ciphertexts = ["SSBoYXZlIG1ldCB0aGVtIGF0IGNsb3NlIG9mIGRheQ==",
"Q29taW5nIHdpdGggdml2aWQgZmFjZXM=",
"RnJvbSBjb3VudGVyIG9yIGRlc2sgYW1vbmcgZ3JleQ==",
"RWlnaHRlZW50aC1jZW50dXJ5IGhvdXNlcy4=",
"SSBoYXZlIHBhc3NlZCB3aXRoIGEgbm9kIG9mIHRoZSBoZWFk",
"T3IgcG9saXRlIG1lYW5pbmdsZXNzIHdvcmRzLA==",
"T3IgaGF2ZSBsaW5nZXJlZCBhd2hpbGUgYW5kIHNhaWQ=",
"UG9saXRlIG1lYW5pbmdsZXNzIHdvcmRzLA==",
"QW5kIHRob3VnaHQgYmVmb3JlIEkgaGFkIGRvbmU=",
"T2YgYSBtb2NraW5nIHRhbGUgb3IgYSBnaWJl",
"VG8gcGxlYXNlIGEgY29tcGFuaW9u",
"QXJvdW5kIHRoZSBmaXJlIGF0IHRoZSBjbHViLA==",
"QmVpbmcgY2VydGFpbiB0aGF0IHRoZXkgYW5kIEk=",
"QnV0IGxpdmVkIHdoZXJlIG1vdGxleSBpcyB3b3JuOg==",
"QWxsIGNoYW5nZWQsIGNoYW5nZWQgdXR0ZXJseTo=",
"QSB0ZXJyaWJsZSBiZWF1dHkgaXMgYm9ybi4=",
"VGhhdCB3b21hbidzIGRheXMgd2VyZSBzcGVudA==",
"SW4gaWdub3JhbnQgZ29vZCB3aWxsLA==",
"SGVyIG5pZ2h0cyBpbiBhcmd1bWVudA==",
"VW50aWwgaGVyIHZvaWNlIGdyZXcgc2hyaWxsLg==",
"V2hhdCB2b2ljZSBtb3JlIHN3ZWV0IHRoYW4gaGVycw==",
"V2hlbiB5b3VuZyBhbmQgYmVhdXRpZnVsLA==",
"U2hlIHJvZGUgdG8gaGFycmllcnM/",
"VGhpcyBtYW4gaGFkIGtlcHQgYSBzY2hvb2w=",
"QW5kIHJvZGUgb3VyIHdpbmdlZCBob3JzZS4=",
"VGhpcyBvdGhlciBoaXMgaGVscGVyIGFuZCBmcmllbmQ=",
"V2FzIGNvbWluZyBpbnRvIGhpcyBmb3JjZTs=",
"SGUgbWlnaHQgaGF2ZSB3b24gZmFtZSBpbiB0aGUgZW5kLA==",
"U28gc2Vuc2l0aXZlIGhpcyBuYXR1cmUgc2VlbWVkLA==",
"U28gZGFyaW5nIGFuZCBzd2VldCBoaXMgdGhvdWdodC4=",
"VGhpcyBvdGhlciBtYW4gSSBoYWQgZHJlYW1lZA==",
"QSBkcnVua2VuLCB2YWluLWdsb3Jpb3VzIGxvdXQu",
"SGUgaGFkIGRvbmUgbW9zdCBiaXR0ZXIgd3Jvbmc=",
"VG8gc29tZSB3aG8gYXJlIG5lYXIgbXkgaGVhcnQs",
"WWV0IEkgbnVtYmVyIGhpbSBpbiB0aGUgc29uZzs=",
"SGUsIHRvbywgaGFzIHJlc2lnbmVkIGhpcyBwYXJ0",
"SW4gdGhlIGNhc3VhbCBjb21lZHk7",
"SGUsIHRvbywgaGFzIGJlZW4gY2hhbmdlZCBpbiBoaXMgdHVybiw=",
"VHJhbnNmb3JtZWQgdXR0ZXJseTo=",
"QSB0ZXJyaWJsZSBiZWF1dHkgaXMgYm9ybi4="]

nonce = 0
key = urandom(16)
cipher = AES.new(key, AES.MODE_ECB)

def decrypt(keystream, ciphertext):
	length = len(ciphertext)
	ciphertext = hexlify(ciphertext)
	keystream = keystream[:length]
	plaintext = int(keystream.hex(),16) ^ int(ciphertext.hex(),16)
	return bytes.fromhex("{0:0{1}x}".format(plaintext,32))


def decrypt_text(ciphertexts):

	for i in range(0, len(ciphertext), 16):
	
		c = ciphertext[i:i+16]
		print(c)
		c = c.encode()
		# turn nonce into 16 bit number
		n = b'\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'

		# generate keystream
		keystr = cipher.encrypt(n)
		print(decrypt(keystr, c), end="")
	print("\n")

for ciphertext in ciphertexts:
	decoded_ciphertext = b64decode(ciphertext)
	decrypt_text(decoded_ciphertext)
