import socket
import time

HOST = '192.168.196.148'
PORT = 3000
socket.setdefaulttimeout(3)
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
s.bind((HOST, PORT))
s.listen(12)

client, address = 0, 0
try:
    client, address = s.accept()
    clientConnected  = True
    if(client): 
        client.send(b'?')
        name = client.recv(3).decode('utf-8')
        print(address[0] + ' Connected as ' + name)
except:
    clientConnected  = False
    print('[ error ] Cannot connect to Robot.!!')
cnt = 0
def rotate_arm1(dir, pull):
    data = bytearray()
    data.append(ord('A'))
    data.append(ord('1'))
    data.append(ord(str(dir)))
    data.append(pull%256)
    data.append(int(pull/256))
    client.send(data)
def rotate_arm2(dir):
    data = bytearray()
    data.append(ord('A'))
    data.append(ord('2'))
    data.append(ord(str(dir)))
    data.append(ord('0'))
    data.append(ord('0'))
    client.send(data)
def rotate_arm3(dir):
    data = bytearray()
    data.append(ord('A'))
    data.append(ord('3'))
    data.append(ord(str(dir)))
    data.append(ord('0'))
    data.append(ord('0'))
    client.send(data)
def set_step_home():
    client.send(b'ST')

rotate_arm1(1, 100)


# while True:
#     data = bytearray()
#     data.append(ord(str(cnt)))
#     client.send(data)
#     print(str(data) + ' Sent')
#     time.sleep(1)
#     cnt+=1