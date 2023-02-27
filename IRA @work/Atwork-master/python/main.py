import pygame
import socket
import time
from Robot import *
from Desk import *
from Arm import *

f = open('desks.txt', "rw+")
desks = f.read()
desk = []
cnt = 0
for i in range(len(desks)):
    dx = ''
    dy = ''
    if(desks[i] == '['):
        i += 1
        while(desks[i] != ','):
            dx += desks[i]
            i += 1
        i += 1
        while(desks[i] != ']'):
            dy += desks[i]
            i += 1
        desk.append(Desk(int(dx), int(dy), desks[i+1], cnt))
        cnt += 1

HOST = '192.168.43.210'
PORT = 3000
socket.setdefaulttimeout(3)
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
s.bind((HOST, PORT))
s.listen(12)

pygame.init()
display = pygame.display.set_mode((1100,600))
pygame.display.set_caption('IRA @Work')
clock = pygame.time.Clock()

robot = Robot(s, 500, 300, display)
arm = Arm(s)

robot_img = pygame.image.load('robot.png')
robot.image = pygame.transform.rotozoom(robot_img, 0, 0.25)
background_color = (50,50,50)
crashed = False
mouse_x = 0
mouse_y = 0
key_pressed = False
doScan = False
def run():
    pass

def showDesks():
    for i in range(len(desk)):
        desk[i].show(display)

robot.speed = 120
v = 0
d = 'S'
arm.set()
while not crashed:
    v2 = 130
    v = 150
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            crashed = True
        if event.type == pygame.KEYDOWN:
            key_pressed = True
            if event.key == pygame.K_UP:
                robot.motor(v,v,-v,-v)
            if event.key == pygame.K_DOWN:
                robot.motor(-v,-v,v,v)
            if event.key == pygame.K_LEFT:
                robot.motor(-v,v,v,-v)
            if event.key == pygame.K_RIGHT:
                robot.motor(v,-v,-v,v)
            if event.key == pygame.K_i:
                robot.arm(2, 'R')
            if event.key == pygame.K_k:
                robot.arm(2, 'L')
            if event.key == pygame.K_o:
                robot.arm(3, 'R')
            if event.key == pygame.K_l:
                robot.arm(3, 'L')
            if event.key == pygame.K_u:
                robot.arm(1, -50)
            if event.key == pygame.K_j:
                robot.arm(1, 50)
            if event.key == pygame.K_q:
                robot.motor(-v2,-v2,-v2,-v2)
            if event.key == pygame.K_e:
                robot.motor(v2,v2,v2,v2)
            if event.key == pygame.K_0:
                robot.goto(desk[0])
            if event.key == pygame.K_1:
                robot.goto(desk[1])
            if event.key == pygame.K_2:
                robot.goto(desk[2])
            if event.key == pygame.K_z:
                robot.x = 500;
                robot.y = 300;

        if event.type == pygame.KEYUP:
            key_pressed = False
            robot.motor(0,0,0,0)
            robot.arm(2,'S')
            robot.arm(3,'S')
            robot.update()
            robot.printDistances()
    display.fill(background_color)
    showDesks()
    robot.show()

    pygame.display.update()
    clock.tick(100)
print('Good Bye.!!')
pygame.quit()
quit()
