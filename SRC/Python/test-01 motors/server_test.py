from tkinter import *
import socket
import time

v = 100

HOST = '192.168.104.148'
PORT = 3000
socket.setdefaulttimeout(3)
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
s.bind((HOST, PORT))
s.listen(12)
try:
    client, address = s.accept()
    print(address[0] + ' Connected.')
except:
    print('[ error ] Cannot connect to Robot.!!')

def motor(ml1, ml2, mr2, mr1, d=0):
    b = bytearray()
    b.append(ord('M'))
    if(mr1 >= 0):
        b.append(ord('+'))
        b.append(mr1);
    else:
        b.append(ord('-'))
        b.append(-mr1);
    if(mr2 >= 0):
        b.append(ord('+'))
        b.append(mr2)
    else:
        b.append(ord('-'))
        b.append(-mr2)
    if(ml2 >= 0):
        b.append(ord('+'))
        b.append(ml2)
    else:
        b.append(ord('-'))
        b.append(-ml2)
    if(ml1 >= 0):
        b.append(ord('+'))
        b.append(ml1)
    else:
        b.append(ord('-'))
        b.append(-ml1)
    client.send(b)

def keydown(e):
    if(e.char == 'w'): motor(v, v, -v, -v)
    if(e.char == 's'): motor(-v, -v, v, v)
    if(e.char == 'a'): motor(-v, v, v, -v)
    if(e.char == 'd'): motor(v, -v, -v, v)
    if(e.char == 'W'): motor(255, 255, -255, -255)
    if(e.char == 'S'): motor(-255, -255, 255, 255)
    if(e.char == 'A'): motor(-255, 255, 255, -255)
    if(e.char == 'D'): motor(255, -255, -255, 255)
    if(e.char == 'e' or e.char == 'E'): motor(100, 100, 100, 100)
    if(e.char == 'q' or e.char == 'Q'): motor(-100, -100, -100, -100)
    
def keyup(e):
    motor(0,0,0,0)

root = Tk()
frame = Frame(root, width=100, height=100)
frame.bind("<KeyPress>", keydown)
frame.bind("<KeyRelease>", keyup)
frame.pack()
frame.focus_set()
root.mainloop()