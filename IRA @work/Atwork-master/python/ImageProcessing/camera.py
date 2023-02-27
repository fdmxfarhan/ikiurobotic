import numpy as np
import cv2

class Camera:
    def __init__(self ,cam_num):
        global cap
        cap=cv2.VideoCapture(cam_num)
    def matchTemplate(self,frame):
        img_gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        template = cv2.imread('obj.png',0)
        w, h = template.shape[::-1]
        res = cv2.matchTemplate(img_gray,template,cv2.TM_CCOEFF_NORMED)
        threshold = 0.7
        loc = np.where( res >= threshold)
        for pt in zip(*loc[::-1]):
            cv2.rectangle(frame, pt, (pt[0] + w, pt[1] + h), (0,0,255), 1)
        return frame
    def showObj(self):
        while(True):
            ret, frame = cap.read()
            #frame = cv2.blur(frame,(10,10))
            if np.shape(frame) == ():
                print "Empty frame err!!!"
            else:
                gray = cv2.cvtColor(frame,cv2.COLOR_BGR2GRAY)
                gray = cv2.blur(gray,(10,10))
                ret,thresh = cv2.threshold(gray,110,255,cv2.THRESH_BINARY_INV)

                #Canny = cv2.Canny(thresh, 175, 175)
                image, contours, hierarchy = cv2.findContours(thresh,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
                contours = sorted(contours, key=cv2.contourArea, reverse=True)
                frame2=frame.copy()
                for cnt in contours:
                    x,y,w,h = cv2.boundingRect(cnt) # offsets - with this you get 'mask'
                    thresh2=thresh.copy()
                    cv2.fillPoly(thresh2, pts =[cnt], color=(0,0,0))
                    count = cv2.countNonZero(thresh2[y:y+h,x:x+w])
                    cv2.imshow("obj",frame2[y:y+h,x:x+w])

                    if 50<cv2.contourArea(cnt)<30000 and (count<=5):
                        cv2.rectangle(frame2,(x,y),(x+w,y+h),(0,255,0),2)
                        cv2.drawContours(frame2,[cnt], 0, (0,255,0), 1)

                        center_x=x+(w/2)
                        center_y=y+(h/2)
                        # for i in range(len(cont)):
                        #     frame = cv2.line(frame, (cont[i][0][0],cont[i][0][1]), (x, y), (255,0,0))
                        frame2 = cv2.circle(frame2,(center_x,center_y), 5, (0,0,255), -1)

                cv2.imshow('threshold',thresh)
                #cv2.imshow('Canny',Canny)
                cv2.imshow('frame2',frame2 )
                #frame=self.matchTemplate(frame)
                #cv2.imshow('frame',frame)


                if cv2.waitKey(1) & 0xFF == ord('q'):
                    # print(contours)
                    break

        cap.release()
        cv2.destroyAllWindows()

#############################################
obj=Camera(2)
obj.showObj()
