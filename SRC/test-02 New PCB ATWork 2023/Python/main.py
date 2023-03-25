from tkinter import *
import socket
import cv2
from PIL import Image, ImageTk
import time

speed = 50


HOST = '192.168.147.148'
PORT = 3000
clientConnected  = False
socket.setdefaulttimeout(3)
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
s.bind((HOST, PORT))
s.listen(12)
client, address = 0, 0
try:
    client, address = s.accept()
    clientConnected  = True
    print(address[0] + ' Connected.')
except:
    clientConnected  = False
    print('[ error ] Cannot connect to Robot.!!')
keyHold = False
frontDist = 0
backDist  = 0
rightDist = 0
leftDist  = 0
getDistEN = False
AIStarted = False

root = Tk()
root.geometry("900x700")
frame = Frame(root, width=900, height=700)
cap= cv2.VideoCapture(0)
imgLable = Label(frame)
speedLabelVar = StringVar()
clientLabelVar = StringVar()
frontDistLabelVar = StringVar()
backDistLabelVar = StringVar()
rightDistLabelVar = StringVar()
leftDistLabelVar = StringVar()


def motor(ml1, ml2, mr2, mr1, d=0):
    global clientConnected
    if not clientConnected: 
        return
    b = bytearray()
    b.append(ord('M'))
    if(mr1 >= 0):
        b.append(ord('+'))
        b.append(mr1)
    else:
        b.append(ord('-'))
        b.append(-mr1)
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
    global keyHold, AIStarted
    if not keyHold:
        if(e.char == 'z'): AIStarted = not AIStarted

        if(e.char == 'w'): motor(speed, speed, -speed, -speed)
        if(e.char == 's'): motor(-speed, -speed, speed, speed)
        if(e.char == 'd'): motor(speed, -speed, -speed, speed)
        if(e.char == 'a'): motor(-speed, speed, speed, -speed)
        if(e.char == 'W'): motor(255, 255, -255, -255)
        if(e.char == 'S'): motor(-255, -255, 255, 255)
        if(e.char == 'D'): motor(255, -255, -255, 255)
        if(e.char == 'A'): motor(-255, 255, 255, -255)
        if(e.char == 'e' or e.char == 'E'): motor(50, 50, 50, 50)
        if(e.char == 'q' or e.char == 'Q'): motor(-50, -50, -50, -50)
        keyHold = True
def keyup(e):
    global keyHold
    motor(0,0,0,0)
    keyHold = False
def show_frames():
   cv2image= cv2.cvtColor(cap.read()[1],cv2.COLOR_BGR2RGB)
   img = Image.fromarray(cv2image)
   imgtk = ImageTk.PhotoImage(image = img)
   imgLable.imgtk = imgtk
   imgLable.configure(image=imgtk)
   imgLable.after(10, show_frames)
def move(direction):
    global speed
    if(direction == 0 ):     motor(speed   , speed   , -speed  , -speed   )
    if(direction == 1 ):     motor(speed   , speed/2 , -speed  , -speed/2 )
    if(direction == 2 ):     motor(speed   , 0       , -speed  , 0        )
    if(direction == 3 ):     motor(speed   , -speed/2, -speed  , speed/2  )
    if(direction == 4 ):     motor(speed   , -speed  , -speed  , speed    )
    if(direction == 5 ):     motor(speed/2 , -speed  , -speed/2, speed    )
    if(direction == 6 ):     motor(0       , -speed  , 0       , speed    )
    if(direction == 7 ):     motor(-speed/2, -speed  , speed/2 , speed    )    
    if(direction == 8 ):     motor(-speed  , -speed  , speed   , speed    )
    if(direction == 9 ):     motor(-speed   , -speed/2, speed   , speed/2 )
    if(direction == 10):     motor(-speed   , 0       , speed   , 0       )
    if(direction == 11):     motor(-speed   , speed/2 , speed   , -speed/2)
    if(direction == 12):     motor(-speed   , speed   , speed   , -speed  )
    if(direction == 13):     motor(-speed/2 , speed   , speed/2 , -speed  )
    if(direction == 14):     motor(0        , speed   , 0       , -speed  )
    if(direction == 15):     motor(speed/2  , speed   , -speed/2, -speed  )
def move_forward():  move(0)
def move_backward(): move(8)
def move_right():    move(4)
def move_left():     move(12)
def stop(): motor(0, 0, 0, 0)
def setGY():
    if not clientConnected: 
        return
    client.send(b'S00000000')
def decreaseSpeed():
    global speed, speedLabelVar
    speed -= 1
    speedLabelVar.set('speed: ' + str(speed))
def increaseSpeed():
    global speed, speedLabelVar
    speed += 1
    speedLabelVar.set('speed: ' + str(speed))
def connectToRobot():
    global client, address, clientConnected
    if clientConnected: 
        return
    try:
        client, address = s.accept()
        clientConnected  = True
        print(address[0] + ' Connected.')
    except:
        clientConnected  = False
        print('[ error ] Cannot connect to Robot.!!')
    if(clientConnected): clientLabelVar.set('Connected')
    else:                clientLabelVar.set('Disonnected')
def disconnectFromRobot():
    global client, address, clientConnected
    if clientConnected: 
        client.close()
        clientConnected  = False
    if(clientConnected): clientLabelVar.set('Connected')
    else:                clientLabelVar.set('Disonnected')
def toggleCorrection():
    if not clientConnected: 
        return
    client.send(b'CT0000000')
def updateDistances():
    global frontDist, backDist, rightDist, leftDist
    if clientConnected: 
        client.send(b'DALL00000')
        try:
            rec = client.recv(100)
            if len(rec) == 8:
                frontDist = (rec[0] << 8) | rec[1]
                backDist  = (rec[4] << 8) | rec[5]
                rightDist = (rec[2] << 8) | rec[3]
                leftDist  = (rec[6] << 8) | rec[7]
        except:
            pass
            # print('Failed to recieve data')
    updateDistText()
    # frame.after(100, updateDistances)
def updateDistText():
    global frontDist, backDist, rightDist, leftDist
    frontDistLabelVar.set('front: ' + str(frontDist) + 'cm')
    backDistLabelVar.set('back: ' + str(backDist) + 'cm')
    rightDistLabelVar.set('right: ' + str(rightDist) + 'cm')
    leftDistLabelVar.set('left: ' + str(leftDist) + 'cm')

######################## AI Begin

arrived = False
def robotMainSetup():
    arrived = False
def robotMainLoop():
    global AIStarted, frontDist, backDist, rightDist, leftDist, moving
    # updateDistances()
    if AIStarted:
        if leftDist > 30:
            move_left()
        else:
            stop()
    frame.after(500, robotMainLoop)

######################## AI End

speedLabelVar.set('speed: ' + str(speed))
updateDistText()
if(clientConnected): clientLabelVar.set('Connected')
else:                clientLabelVar.set('Disonnected')
wButton = Button(frame, text =" W ", command = move_forward)
aButton = Button(frame, text =" A ", command = move_left)
sButton = Button(frame, text =" S ", command = move_backward)
dButton = Button(frame, text =" D ", command = move_right)
setGYButton = Button(frame, text =" Set GY 0 Deg ", command = setGY)
stopButton  = Button(frame, text =" Stop ", command = stop)
connectButton  = Button(frame, text =" Connect ", command = connectToRobot)
disconnectButton  = Button(frame, text =" Disonnect ", command = disconnectFromRobot)
plusButton  = Button(frame, text =" + ", command = increaseSpeed)
minusButton = Button(frame, text =" - ", command = decreaseSpeed)
correctionButton  = Button(frame, text =" Toggle Correction ", command = toggleCorrection)

speedLabel  = Label(frame, textvariable = speedLabelVar)
clientLabel  = Label(frame, textvariable = clientLabelVar)
frontDistLabel  = Label(frame, textvariable = frontDistLabelVar)
backDistLabel   = Label(frame, textvariable = backDistLabelVar)
rightDistLabel  = Label(frame, textvariable = rightDistLabelVar)
leftDistLabel   = Label(frame, textvariable = leftDistLabelVar)


show_frames()
robotMainSetup()
robotMainLoop()
frame.bind("<KeyPress>", keydown)
frame.bind("<KeyRelease>", keyup)
wButton.place(x=45, y=520)
aButton.place(x=20, y=560)
sButton.place(x=50, y=560)
dButton.place(x=80, y=560)
setGYButton.place(x=150, y=520)
stopButton.place(x=150, y=560)
speedLabel.place(x = 290, y = 520)
plusButton.place(x = 360, y = 520)
minusButton.place(x = 260, y = 520)
correctionButton.place(x=210, y=560)


imgLable.place(x=10, y=15)

connectButton.place(x=750, y=15)
disconnectButton.place(x=820, y=15)
clientLabel.place(x=660, y=18)
frontDistLabel.place(x=660, y=70)
backDistLabel.place(x=660, y=90)
rightDistLabel.place(x=660, y=110)
leftDistLabel.place(x=660, y=130)

frame.pack()
frame.focus_set()
root.mainloop()