"""
A simple Python script to receive messages from a client over
Bluetooth using Python sockets (with Python 3.3 or above).
"""

import socket
import os
import time

hostMACAddress = 'B8:27:EB:C4:3E:8C' # The MAC address of a Bluetooth adapter on the server. The server might have multiple Bluetooth adapters.
port = 3 # 3 is an arbitrary choice. However, it must match the port used by the client.
backlog = 1
size = 1024
s = socket.socket(socket.AF_BLUETOOTH, socket.SOCK_STREAM, socket.BTPROTO_RFCOMM)
s.bind((hostMACAddress,port))
s.listen(backlog)
file = open("reciveBT","w")
file.close()
print(os.stat("reciveBT").st_size)
try:
    client, address = s.accept()
    while 1:
        data = client.recv(size)
       
        if data:
           
            
            if os.stat("reciveBT").st_size >= 0:
                file = open("reciveBT", "w")
                print("3")
                time.sleep(0.000001)
                file.write(str(data))
                client.send(data)
                print(data)
            file.close()
            print("efter close")
            print(os.stat("reciveBT").st_size)
        
            
except:	
    print("Closing socket")	
    client.close()
    s.close()
    #file = open("Mottaget.txt", "w")
    #file.close()
