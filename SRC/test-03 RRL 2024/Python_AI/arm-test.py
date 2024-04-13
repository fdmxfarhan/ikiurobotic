import socket
import time
HOST = '192.168.208.148'
PORT = 3000
MovConnected  = False
ArmConnected  = False
socket.setdefaulttimeout(3)
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
s.bind((HOST, PORT))
s.listen(12)


client, address = s.accept()
def motor(v1, v2, v3, v4):
    global client
    b = bytearray()
    b.append(ord('A'))
    b.append(v1//255)
    b.append(v1%255)
    b.append(v2//255)
    b.append(v2%255)
    b.append(v3//255)
    b.append(v3%255)
    b.append(v4//255)
    b.append(v4%255)
    client.send(b)
    time.sleep(0.1)

def servo1(angle):
    global client
    b = bytearray()
    b.append(ord('S'))
    b.append(ord('1'))
    b.append(angle//255)
    b.append(angle%255)
    b.append(ord('-'))
    b.append(ord('-'))
    b.append(ord('-'))
    b.append(ord('-'))
    b.append(ord('-'))
    client.send(b)
    time.sleep(0.1)
    
def servo2(angle):
    global client
    b = bytearray()
    b.append(ord('S'))
    b.append(ord('2'))
    b.append(angle//255)
    b.append(angle%255)
    b.append(ord('-'))
    b.append(ord('-'))
    b.append(ord('-'))
    b.append(ord('-'))
    b.append(ord('-'))
    client.send(b)
    time.sleep(0.1)
time.sleep(0.1)
servo1(100)
servo2(200)

motor(3800, 3500, 2000, 0)
# motor(1600, 2100, 1800, 0)
time.sleep(0.1)
# data = client.recv(100)
# print(data)