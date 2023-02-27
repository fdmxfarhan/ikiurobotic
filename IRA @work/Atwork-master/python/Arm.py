import pygame
import socket
import time

class Arm():
    def __init__(self, s):
        # try:
        #     client, address = s.accept()
        #     if(address[0] == '192.168.43.41'):
        #         print('Arm ' + address[0] + ' Connected.')
        #         self.available = True
        #         self.client = client
        #     else:
        #         client.close()
        #         client, address = s.accept()
        #         if(address[0] == '192.168.43.41'):
        #             print('Arm ' + address[0] + ' Connected.')
        #             self.available = True
        #             self.client = client
        #         else:
        #             client.close()
        #             self.available = False
        #             self.client = 0
        # except:
        #     print('[ error ] Cannot connect to Arm.!!')
        self.available = False
        self.client = 0
        self.angle = 0
    def set(self):
        if(not self.available):
            return False
        self.client.send(b'S')
    def update(self):
        if(not self.available):
            return False
        time.sleep(0.01)
        self.client.send(b'G')
        time.sleep(0.01)
        rec = self.client.recv(100)
        # print(rec)
        if(rec == '-'):
            pass
        else:
            try:
                self.angle = int(rec)
            except:
                print('Error')
                self.update()
        if(self.angle > 180):
            self.angle -= 360
        if(self.angle < -180):
            self.angle += 360

        if(self.angle > 0 and self.angle < 10):
            self.cmp = 30
        elif(self.angle < 0 and self.angle > -10):
            self.cmp = -30
        elif(self.angle >= 10 and self.angle < 45):
            self.cmp = 50
        elif(self.angle <= -10 and self.angle > -45):
            self.cmp = -50
        elif(self.angle >= 45):
            self.cmp = 100
        elif(self.angle <= -45):
            self.cmp = -100
        else:
            self.cmp = 0
        # print(self.angle)
    def printAll(self):
        print('[ GY       ] ' + str(self.angle))
    def rotate(self, a):
        b = bytearray()
        if(a >= 0):
            for i in range(abs(a)):
                b.append('R')
            print('[ Arm      ] rotation ' + str(a) + ' right')
        else:
            for i in range(abs(a)):
                b.append('L')
            print('[ Arm      ] rotation ' + str(-a) + ' left')
        self.client.send(b)
