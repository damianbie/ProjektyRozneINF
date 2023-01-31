from re import S
import socket
import time


HOST = "192.168.1.162"  # The server's hostname or IP address
PORT = 3333  # The port used by the server

print(f"Połączenie z {HOST}:{PORT}")

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    while True:
        dataToSend = str(input())
        start = time.time()
        s.send(str.encode(dataToSend))
        dataRec = s.recv(1024)
        # print("Received: " + dataRec.decode("utf-8"))
        print("Received: " + str(dataRec))
        end = time.time()
        print(f"Czas {end - start}")

