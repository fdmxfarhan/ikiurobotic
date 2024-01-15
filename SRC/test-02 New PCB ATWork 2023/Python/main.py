from tkinter import *
import socket
import cv2
from PIL import Image, ImageTk
import time

speed = 50


HOST = '192.168.46.148'
PORT = 3000
MovConnected  = False
ArmConnected  = False
socket.setdefaulttimeout(3)
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
s.bind((HOST, PORT))
s.listen(12)
MovClient, MovAddress = 0, 0
ArmClient, ArmAddress = 0, 0

keyHold = False
frontDist = 0
backDist  = 0
rightDist = 0
leftDist  = 0
getDistEN = False
AIStarted = False

root = Tk()
root.geometry("900x700")
defaultbg = root.cget('bg')
frame = Frame(root, width=900, height=700)
cap= cv2.VideoCapture(0)
imgLable = Label(frame)
speedLabelVar = StringVar()
MovClientLabelVar = StringVar()
ArmClientLabelVar = StringVar()
frontDistLabelVar = StringVar()
backDistLabelVar = StringVar()
rightDistLabelVar = StringVar()
leftDistLabelVar = StringVar()
StateLabelVar = StringVar()
StateLabelVar.set('Manual')

def motor(ml1, ml2, mr2, mr1, d=0):
    global MovConnected
    if not MovConnected: 
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
    MovClient.send(b)
    time.sleep(0.01)
def keydown(e):
    global keyHold, AIStarted
    if not keyHold:
        if(e.char == 'z'): 
            AIStarted = not AIStarted
            if AIStarted:   
                StateLabelVar.set('AI')
                StateLabel.config(fg='red')
                robotMainSetup()
            else:           
                StateLabelVar.set('Manual')
                StateLabel.config(fg='green')
            
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
        if(e.char == 'x'): set_step_home()
        if(e.char == 'u'): rotate_arm1(1, 100)
        if(e.char == 'o'): rotate_arm1(0, 100)
        if(e.char == 'c'): updateDistances()
        keyHold = True
    if(e.char == 'j'): rotate_arm2(0)
    if(e.char == 'l'): rotate_arm2(1)
    if(e.char == 'i'): rotate_arm3(0)
    if(e.char == 'k'): rotate_arm3(1)
def keyup(e):
    global keyHold
    if(e.char == 'j'): rotate_arm2(2)
    if(e.char == 'l'): rotate_arm2(2)
    if(e.char == 'i'): rotate_arm3(2)
    if(e.char == 'k'): rotate_arm3(2)
    else:
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
    if not MovConnected: 
        return
    MovClient.send(b'S00000000')
def decreaseSpeed():
    global speed, speedLabelVar
    speed -= 1
    speedLabelVar.set('speed: ' + str(speed))
def increaseSpeed():
    global speed, speedLabelVar
    speed += 1
    speedLabelVar.set('speed: ' + str(speed))
def movConnectToRobot():
    global MovClient, MovAddress, MovConnected
    if MovConnected: 
        return
    for i in range(2):
        try:
            client, address = s.accept()
            if(client):
                client.send(b'?')
                name = client.recv(3).decode('utf-8')
                if(name == 'MOV'):
                    MovClient = client
                    MovAddress = address
                    MovConnected = True
                    print(address[0] + ' Connected as ' + name)
                    MovClientLabelVar.set('Connected')
                    return
                else: 
                    client.close()
        except:
            MovConnected  = False
            # print('target ' + str(i) + ' not found')
    if(MovConnected):    MovClientLabelVar.set('Connected')
    else:                MovClientLabelVar.set('Disonnected')
def movDisconnectFromRobot():
    global MovClient, MovAddress, MovConnected
    if MovConnected: 
        MovClient.close()
        MovConnected  = False
        print('MOV Disconnected')
    if(MovConnected): MovClientLabelVar.set('Connected')
    else:                MovClientLabelVar.set('Disonnected')
def armConnectToRobot():
    global ArmClient, ArmAddress, ArmConnected
    if ArmConnected: 
        return
    for i in range(2):
        try:
            client, address = s.accept()
            if(client):
                client.send(b'?')
                name = client.recv(3).decode('utf-8')
                if(name == 'ARM'):
                    ArmClient = client
                    ArmAddress = address
                    ArmConnected = True
                    print(address[0] + ' Connected as ' + name)
                    ArmClientLabelVar.set('Connected')
                    return
                else: 
                    client.close()
        except:
            ArmConnected  = False
            # print('target ' + str(i) + ' not found')
    if(ArmConnected):    ArmClientLabelVar.set('Connected')
    else:                ArmClientLabelVar.set('Disonnected')
def armDisconnectFromRobot():
    global ArmClient, ArmAddress, ArmConnected
    if ArmConnected: 
        ArmClient.close()
        ArmConnected  = False
        print('ARM Disconnected')
    if(ArmConnected):    ArmClientLabelVar.set('Connected')
    else:                ArmClientLabelVar.set('Disonnected')
def toggleCorrection():
    if not MovConnected: 
        return
    MovClient.send(b'CT0000000')
def updateDistances():
    global frontDist, backDist, rightDist, leftDist
    if MovConnected: 
        MovClient.send(b'DALL00000')
        time.sleep(0.01)
        try:
            rec = MovClient.recv(100)
            time.sleep(0.01)
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
def rotate_arm1(dir, pull):
    if not ArmConnected:
        print('ARM Node Not Connected !!')
        return
    data = bytearray()
    data.append(ord('A'))
    data.append(ord('1'))
    data.append(ord(str(dir)))
    data.append(pull%256)
    data.append(int(pull/256))
    ArmClient.send(data)
def rotate_arm2(dir):
    if not ArmConnected:
        print('ARM Node Not Connected !!')
        return
    data = bytearray()
    data.append(ord('A'))
    data.append(ord('2'))
    data.append(ord(str(dir)))
    data.append(ord('0'))
    data.append(ord('0'))
    ArmClient.send(data)
def rotate_arm3(dir):
    if not ArmConnected:
        print('ARM Node Not Connected !!')
        return
    data = bytearray()
    data.append(ord('A'))
    data.append(ord('3'))
    data.append(ord(str(dir)))
    data.append(ord('0'))
    data.append(ord('0'))
    ArmClient.send(data)
def set_step_home():
    if not ArmConnected:
        print('ARM Node Not Connected !!')
        return
    ArmClient.send(b'ST')
def Nbutton_call_back():
    if not MovConnected: 
        return
    MovClient.send(b'PN000000')
    time.sleep(0.01)
def Wbutton_call_back():
    if not MovConnected: 
        return
    MovClient.send(b'PW000000')
    time.sleep(0.01)
def Sbutton_call_back():
    if not MovConnected: 
        return
    MovClient.send(b'PS000000')
    time.sleep(0.01)
def Ebutton_call_back():
    if not MovConnected: 
        return
    MovClient.send(b'PE000000')
    time.sleep(0.01)
######################## AI Begin

arrived = False
moving = False
def robotMainSetup():
    arrived = False
    moving = False
    stop() 
def robotMainLoop():
    global AIStarted, frontDist, backDist, rightDist, leftDist, moving
    if AIStarted:
        updateDistances()
        # if backDist > 60:
        #     if not moving: 
        #         move_backward()
        #         moving = True
        # else:
        #     stop()
        #     moving = False
    else:
        pass
    frame.after(500, robotMainLoop)

######################## AI End

speedLabelVar.set('speed: ' + str(speed))
updateDistText()
if(MovConnected): MovClientLabelVar.set('Connected')
else:                MovClientLabelVar.set('Disonnected')
if(MovConnected): ArmClientLabelVar.set('Connected')
else:                ArmClientLabelVar.set('Disonnected')
wButton = Button(frame, text =" W ", command = move_forward)
aButton = Button(frame, text =" A ", command = move_left)
sButton = Button(frame, text =" S ", command = move_backward)
dButton = Button(frame, text =" D ", command = move_right)
setGYButton = Button(frame, text =" Set GY 0 Deg ", command = setGY)
stopButton  = Button(frame, text =" Stop ", command = stop)

MovConnectButton  = Button(frame, text =" Connect ", command = movConnectToRobot)
MovDisconnectButton  = Button(frame, text =" Disonnect ", command = movDisconnectFromRobot)
MovClientLabel  = Label(frame, textvariable = MovClientLabelVar)
MovTitleLabel  = Label(frame, text = "MOV Node:")

ArmConnectButton  = Button(frame, text =" Connect ", command = armConnectToRobot)
ArmDisconnectButton  = Button(frame, text =" Disonnect ", command = armDisconnectFromRobot)
ArmClientLabel  = Label(frame, textvariable = ArmClientLabelVar)
ArmTitleLabel  = Label(frame, text = "ARM Node:")

StateLabel  = Label(frame, textvariable= StateLabelVar, font=("Tahoma", 17))
StateLabel.config(fg='green')

plusButton  = Button(frame, text =" + ", command = increaseSpeed)
minusButton = Button(frame, text =" - ", command = decreaseSpeed)

NButton  = Button(frame, text =" N ", command = Nbutton_call_back)
WButton  = Button(frame, text =" W ", command = Wbutton_call_back)
SButton  = Button(frame, text =" S ", command = Sbutton_call_back)
EButton  = Button(frame, text =" E ", command = Ebutton_call_back)




correctionButton  = Button(frame, text =" Toggle Correction ", command = toggleCorrection)
speedLabel  = Label(frame, textvariable = speedLabelVar)
frontDistLabel  = Label(frame, textvariable = frontDistLabelVar)
backDistLabel   = Label(frame, textvariable = backDistLabelVar)
rightDistLabel  = Label(frame, textvariable = rightDistLabelVar)
leftDistLabel   = Label(frame, textvariable = leftDistLabelVar)


show_frames()
# robotMainSetup()
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


NButton.place(x=500, y=520)
WButton.place(x=469, y=550)
SButton.place(x=500, y=580)
EButton.place(x=530, y=550)



imgLable.place(x=10, y=15)

MovConnectButton.place(x=750, y=45)
MovDisconnectButton.place(x=820, y=45)
MovClientLabel.place(x=660, y=48)
MovTitleLabel.place(x=760, y=20)

ArmConnectButton.place(x=750, y=115)
ArmDisconnectButton.place(x=820, y=115)
ArmClientLabel.place(x=660, y=118)
ArmTitleLabel.place(x=760, y=90)

frontDistLabel.place(x=660, y=180)
backDistLabel.place(x=660, y=200)
rightDistLabel.place(x=660, y=220)
leftDistLabel.place(x=660, y=240)
StateLabel.place(x=800, y=650)
frame.pack()
frame.focus_set()
root.mainloop()