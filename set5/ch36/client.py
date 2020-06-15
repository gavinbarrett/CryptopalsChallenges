import socket

s = socket.socket()
s.connect(('127.0.0.1', 5000))
user = 'gavin100:- '
while True:
	i = input('> ')
	s.send((user + i).encode())

s.close()
