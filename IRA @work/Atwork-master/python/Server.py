import socket
import time
HOST = '192.168.43.210'
PORT = 3000

socket.setdefaulttimeout(0.1)
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
s.bind((HOST, PORT))
s.listen(1)

client = []
clientName = []

for i in range(100):
    try:
        client.append(s.accept())
    except:
        pass
print(str(len(client)) + ' Client Connected.')
for i in range(len(client)):
    client[i][0].send(b'N')
    time.sleep(0.5)
    clientName.append(client[i][0].recv(100).decode("utf-8"))
print(clientName)
s.close()
