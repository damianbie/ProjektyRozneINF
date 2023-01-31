from re import S
import socket
import time
import sys
# import fcntl, os
import keyboard
import errno

HOST = "192.168.1.162"  # The server's hostname or IP address
HOST = "192.168.4.1"  # The server's hostname or IP address
PORT = 3333  # The port used by the server
MAX_BYTE = 1024

print(f"Połączenie z {HOST}:{PORT}")

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    # s.setblocking(False)
    while True:
        if keyboard.read_key() != 0:
            dataToSend = str(input())
            s.send(str.encode(dataToSend))

        print("Odebrane => ", end="")
        print((s.recv(MAX_BYTE)).decode("ascii"))
