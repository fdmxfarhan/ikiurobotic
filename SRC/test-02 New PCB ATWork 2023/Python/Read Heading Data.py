import socket, time, pandas
import pandas as pd
import openpyxl

HOST = '192.168.27.148'
PORT = 3000
socket.setdefaulttimeout(3)
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
s.bind((HOST, PORT))
s.listen(12)

client, address = s.accept()
print(address)
data = []
t = 0.0
a = input()
set_point = 30
while t < 20:
    client.send(b'H00000000')
    time.sleep(0.3)
    rec = client.recv(100)
    thetaD = (rec[0] << 8) | rec[1] - 360 + 256 + set_point
    data.append([t, thetaD, set_point])
    print(thetaD)


    t+=0.3
df = pd.DataFrame(data, columns=['time', 'thetaD', 'SetPoint'])
df.to_excel('thetaD_data.xlsx', sheet_name='thetaD')


client.close()