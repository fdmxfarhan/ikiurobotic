import serial
import cv2
import numpy as np
from math import *

w = 700
h = 1000
img = np.zeros((w, h, 3), dtype = np.uint8)
white = [255, 255, 255]
ser = serial.Serial('COM27', 230400)
lidarDist = []
while True:
    data = ser.read()[0]
    frame = []
    if data == 0x54:
        verLen = ser.read()[0]
        for i in range(verLen+1):
            data = ser.read()[0]
            frame.append(data)
        speed      =  frame[1] << 8 | frame[0]
        startAngle = (frame[3] << 8 | frame[2])/100.0
        
        distance  = []
        intensity = []
        num = int((verLen - 10)/4)
        for i in range(num):
            distance.append( frame[4*i+5] << 8 | frame[4*i+4])
            intensity.append(frame[4*i+7] << 8 | frame[4*i+6])
        endAngle = (frame[verLen-3] << 8 | frame[verLen-4])/100.0
        timeStamp = (frame[verLen-1] << 8 | frame[verLen-2])
        step = (endAngle - startAngle)/num
        if startAngle < 10:
            lidarDist = []
        angle = startAngle
        i=0
        while i < len(distance):
            lidarDist.append([angle, distance[i]])
            i += 1
            angle += step

    # img = np.zeros((w, h, 3), dtype = np.uint8)
    for i in range(len(lidarDist)):
        x = int(w/2 + cos(lidarDist[i][0]) * lidarDist[i][1] * 0.5)
        y = int(h/2 + sin(lidarDist[i][0]) * lidarDist[i][1] * 0.5)
        if x < 0: x = 0
        if x > w-1: x = w-1
        if y < 0: y = 0
        if y > h-1: y = h-1
        img[x][y] = white

    cv2.imshow('black image', img)
    key = cv2.waitKey(1)
    if key & 0xff == 27:
        break
ser.close()