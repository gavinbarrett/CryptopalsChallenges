import socket

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:

	s.bind(("127.0.0.1",5000))

	s.listen()

	conn, addr = s.accept()

	with conn:
		print(f"Connected by {addr}")
		while True:
			data = conn.recv(1024)
			print(data)
			if not data:
				break
			conn.sendall(data)
