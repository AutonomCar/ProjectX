"""
A simple Python script to send messages to a sever over Bluetooth using
Python sockets (with Python 3.3 or above).
"""

import socket
import os
import time

serverMACAddress = 'B8:27:EB:C4:3E:8C' #adress of the device you want to connect to
port = 3
s = socket.socket(socket.AF_BLUETOOTH, socket.SOCK_STREAM, socket.BTPROTO_RFCOMM)
s.connect((serverMACAddress,port))


   
while 1:
    
    file = open("sendBT", "r+")

    for line in file:
        text = line.rstrip('\n')
        time.sleep(0.3)
        print (text)
        if text == "quit":
            break
        s.send(bytes(text, 'UTF-8'))
        file.seek(0)
        file.truncate()

    file.close()

        
print("slut")
s.close()
file.close()
