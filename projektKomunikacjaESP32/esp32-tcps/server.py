import socket
import time

# sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
# sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
# sock.bind(('', 3333))
# sock.listen(5)

# print("Oczekiwanie na połączenie klienta")
# try:
#     while True:
#         newSocket, address = sock.accept(  )
#         print (f"ip: {address}")
#         while True:
#             receivedData = newSocket.recv(100)
#             print(receivedData.decode("ascii"))
            
#             print("tak")
#             newSocket.send(b'Potwierdzenie\0')
#             time.sleep(1)
#         newSocket.close(  )
# finally:
#     sock.close(  )
server_socket = socket.socket()  # get instance
    # look closely. The bind() function takes tuple as argument
server_socket.bind(("", 3333))  # bind host address and port together

# configure how many client the server can listen simultaneously
server_socket.listen(2)
conn, address = server_socket.accept()  # accept new connection
conn.setblocking(True)
print("Connection from: " + str(address))
while True:
    # receive data stream. it won't accept data packet greater than 1024 bytes
    data = conn.recv(1024).decode()
    if not data:
        # if data is not received break
        break
    print("from connected user: " + str(data))
    # data = input(' -> ')
    # conn.send(data.encode())  # send data to the client
    conn.sendall(data.encode())

conn.close()  # close the connection
