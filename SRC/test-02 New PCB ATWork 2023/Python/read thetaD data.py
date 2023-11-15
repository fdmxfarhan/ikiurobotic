import socket, time, pandas
import pandas as pd
import openpyxl

HOST = '192.168.37.148'
PORT = 3000
socket.setdefaulttimeout(1)
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
s.bind((HOST, PORT))
s.listen(12)

client, address = s.accept()
print(address)
data = []
t = 0.0
client.send(b'CT0000000')
time.sleep(0.3)
while t < 120:
    client.send(b'H00000000')
    time.sleep(0.3)
    rec = client.recv(100)
    Heading = (rec[0] << 8) | rec[1] - 360 + 256 
    thetaD = (rec[2] << 8) | rec[3] - 360 + 256 

    data.append([t, Heading, thetaD])
    print(f"Heading:{Heading}\tthetaD:{thetaD}")


    t+=0.3
df = pd.DataFrame(data, columns=['time', 'Heading', 'SetPoint'])
df.to_excel('Heading_data.xlsx', sheet_name='Heading')


client.close()