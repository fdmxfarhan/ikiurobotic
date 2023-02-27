import pygame
import socket
import time

class Robot():
    def __init__(self, s, x, y, display):
        try:
            client, address = s.accept()
            if(address[0] == '192.168.43.75'):
                print('Robot ' + address[0] + ' Connected.')
                self.available = True
                self.client = client
            else:
                client.close()
                client, address = s.accept()
                if(address[0] == '192.168.43.75'):
                    print('Robot ' + address[0] + ' Connected.')
                    self.available = True
                    self.client = client
                else:
                    client.close()
                    self.available = False
                    self.client = 0
        except:
            print('[ error ] Cannot connect to Robot.!!')
            self.available = False
            self.client = 0
        self.x = x
        self.y = y
        self.front = 100
        self.back = 100
        self.right = 100
        self.left = 100
        self.v = 0
        self.cnt = 0
        self.speed = 100
        self.display = display
        self.map = [[0]*1100]*600
    def show(self):
        rect = self.image.get_rect()
        width = rect[2]
        height = rect[3]
        # pygame.draw.rect(display, (50,50,50),(self.x - width/2, self.y - height/2, width, self.front))

        pygame.draw.rect(self.display, (200,100,255),(self.x - width/2,self.y - (self.front + height/2) - 10, width, 10))
        pygame.draw.rect(self.display, (200,100,255),(self.x - width/2,self.y + (self.back + height/2), width, 10))
        pygame.draw.rect(self.display, (200,100,255),(self.x - (self.left + width/2) - 10,self.y - height/2, 10, height))
        pygame.draw.rect(self.display, (200,100,255),(self.x + (self.right + width/2),self.y - height/2, 10, height))
        self.display.blit(self.image, (self.x - width/2, self.y - height/2))
    def recconect(self):
        try:
            client, address = s.accept()
            print(address[0] + ' Connected.')
            self.available = True
            self.client = client
        except:
            print('Cannot connect to Robot.!!')
            self.available = False
            self.client = 0
    def update(self):
        if(not self.available):
            return
        try:
            self.client.send(b'f')
            time.sleep(0.01)
            self.front = int(self.client.recv(10))
            time.sleep(0.01)
            self.client.send(b'b')
            time.sleep(0.01)
            self.back = int(self.client.recv(10))
            time.sleep(0.01)
            self.client.send(b'l')
            time.sleep(0.01)
            self.left = int(self.client.recv(10))
            time.sleep(0.01)
            self.client.send(b'r')
            time.sleep(0.01)
            self.right = int(self.client.recv(10))
        except:
            pass
    def printDistances(self):
        print('[ Distance ]  F:' + str(self.front) + ' R:' + str(self.right) + ' B:' + str(self.back) + ' L:' + str(self.left))
    def motor(self, ml1, ml2, mr2, mr1, d=0):
        if(not self.available):
            print('Robot is not connected.!!')
            return
        b = bytearray()
        b.append('M')
        if(mr1 >= 0):
            b.append('+')
            b.append(mr1);
        else:
            b.append('-')
            b.append(-mr1);
        if(mr2 >= 0):
            b.append('+')
            b.append(mr2)
        else:
            b.append('-')
            b.append(-mr2)
        if(ml2 >= 0):
            b.append('+')
            b.append(ml2)
        else:
            b.append('-')
            b.append(-ml2)
        if(ml1 >= 0):
            b.append('+')
            b.append(ml1)
        else:
            b.append('-')
            b.append(-ml1)
        b.append(d);

        self.client.send(b)
        # cnt = 0
        # while cnt < 4:
        #     try:
        #         rec = self.client.recv(1)
        #         if(rec == 'N'):
        #             return True
        #     except:
        #         pass
        #     cnt+=1
    def move(self, direction, d = 0):
        # if(self.cnt%3 == 0):
        #     if(self.v < self.speed):
        #         self.v += 3
        # self.cnt += 1
        self.v = self.speed
        if(d < 0): d = 0
        if(direction == 0):
            self.motor(self.v, self.v, -self.v, -self.v, d)
            if(d != 0):
                self.y -= d
        elif(direction == 1):
            self.motor(self.v, self.v/2, -self.v, -self.v/2, d)
        elif(direction == 2):
            self.motor(self.v, 0, -self.v, 0, d)
        elif(direction == 3):
            self.motor(self.v, -self.v/2, -self.v, self.v/2, d)
        elif(direction == 4):
            self.motor(self.v, -self.v, -self.v, self.v, d )
            if(d != 0):
                self.x += d
        elif(direction == 5):
            self.motor(self.v/2, -self.v, -self.v/2, self.v, d)
        elif(direction == 6):
            self.motor(0, -self.v, 0, self.v, d)
        elif(direction == 7):
            self.motor(-self.v/2, -self.v, self.v/2, self.v, d)
        elif(direction == 8):
            self.motor(-self.v, -self.v, self.v, self.v, d)
            if(d != 0):
                self.y += d
        elif(direction == 9):
            self.motor(-self.v, -self.v/2, self.v, self.v/2, d)
        elif(direction == 10):
            self.motor(-self.v, 0, self.v, 0, d)
        elif(direction == 11):
            self.motor(-self.v, self.v/2, self.v, -self.v/2, d)
        elif(direction == 12):
            self.motor(-self.v, self.v, self.v, -self.v, d + int(d*0.3))
            if(d != 0):
                self.x -= d
        elif(direction == 13):
            self.motor(-self.v/2, self.v, self.v/2, -self.v, d)
        elif(direction == 14):
            self.motor(0, self.v, 0, -self.v, d)
        elif(direction == 15):
            self.motor(self.v/2, self.v, -self.v/2, -self.v, d)
    def arm(self, number, direction):
        if(not self.available):
            return
        self.client.send(b'A')
        b = bytearray()
        b.append(str(number))
        if(number == 1):
            if(direction<0):
                b.append('-')
                b.append(-direction)
            else:
                b.append('+')
                b.append(direction)
            b.append('a')
        else:
            b.append(direction)
        self.client.send(b)
    def stop2(self):
        self.motor(0, 0, 0, 0)
        self.v = 0
        self.cnt = 0
    def stop(self, arm):
        if(arm.available):
            self.motor(0, 0, 0, 0)
            for i in range(10):
                arm.update()
            while(arm.cmp != 0):
                self.motor(arm.cmp, arm.cmp, arm.cmp, arm.cmp)
                arm.update()
        self.motor(0, 0, 0, 0)
        self.v = 0
        self.cnt = 0
    def goto(self, destination):
        self.stop2()
        direction = 'F'
        min_distance = 15
        if(destination.y > self.y):
            direction = 'B'
        while(self.x != destination.x or self.y != destination.y):
            self.update()
            if(direction == 'F'):
                if(abs(destination.y - self.y) < 15):
                    if(self.front > self.back):
                        self.move(0, self.front/2)
                    else:
                        self.move(8, self.back/2)
                elif(self.front < abs(destination.y - self.y)):
                    self.move(0, self.front - min_distance)
                else:
                    self.move(0, abs(self.y - destination.y))
                if(self.x < destination.x):
                    direction = 'R'
                else:
                    direction = 'L'
            elif(direction == 'B'):
                if(abs(destination.y - self.y) < 15):
                    if(self.front > self.back):
                        self.move(0, self.front/2)
                    else:
                        self.move(8, self.back/2)
                elif(self.back < abs(destination.y - self.y)):
                    self.move(8, self.back - min_distance)
                else:
                    self.move(8, abs(self.y - destination.y))
                if(self.x < destination.x):
                    direction = 'R'
                else:
                    direction = 'L'
            elif(direction == 'R'):
                if(abs(destination.x - self.x) < 15):
                    if(self.left > self.right):
                        self.move(12, self.left/2)
                    else:
                        self.move(4, self.right/2)
                elif(self.right < abs(destination.x - self.x)):
                    self.move(4, self.right - min_distance)
                else:
                    self.move(4, abs(self.x - destination.x))
                if(self.y < destination.y):
                    direction = 'B'
                else:
                    direction = 'F'
            elif(direction == 'L'):
                if(abs(destination.x - self.x) < 15):
                    if(self.left > self.right):
                        self.move(12, self.left/2)
                    else:
                        self.move(4, self.right/2)
                elif(self.left < abs(destination.x - self.x)):
                    self.move(12, self.left - min_distance)
                else:
                    self.move(12, abs(self.x - destination.x))
                if(self.y < destination.y):
                    direction = 'B'
                else:
                    direction = 'F'
            self.show()
        self.stop2()
