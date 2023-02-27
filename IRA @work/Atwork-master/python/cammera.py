import numpy as np
import cv2

cap = cv2.VideoCapture(2)
x = 0
y = 0

while(True):
    ret, frame = cap.read()

    frame = cv2.blur(frame,(10,10))
    gray = cv2.cvtColor(frame,cv2.COLOR_BGR2GRAY)
    ret,thresh = cv2.threshold(gray,110,255,cv2.THRESH_BINARY_INV)
    image, contours, hierarchy = cv2.findContours(thresh,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
    frame = cv2.drawContours(frame, contours, -1, (0,255,0), 1)
    for j in range(len(contours)):
        cont = contours[j]
        x = 0
        y = 0
        for i in range(len(cont)):
            x += cont[i][0][0]
            y += cont[i][0][1]
        # print(x, y)
        x /= len(cont)
        y /= len(cont)
        # for i in range(len(cont)):
        #     frame = cv2.line(frame, (cont[i][0][0],cont[i][0][1]), (x, y), (255,0,0))
        frame = cv2.circle(frame,(x,y), 5, (0,0,255), -1)



    cv2.imshow('threshold',thresh)
    # cv2.imshow('Canny',Canny)
    cv2.imshow('frame',frame )


    if cv2.waitKey(1) & 0xFF == ord('q'):
        # print(contours)
        break

cap.release()
cv2.destroyAllWindows()
