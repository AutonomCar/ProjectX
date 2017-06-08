# Arvid Englund
# 2017-05-23
#
# Can-bus communication
# Project: Autonoma Elbilar
#
#
#   This program should be started parallel with the
#   main pogram. It continuallt fetches data from the
#   CAN-bus and writes it to a file for the main
#   program to read.
#
#   There is a lot of formatting to
#   make sure the c++ program can easily
#   read the string.
#




import can

# Create the socket
bus = can.interface.Bus(channel='can0', bustype='socketcan_native')



while(1):
    print("recieving...")
    arr = ""
    count = 0
    x = 0
    for msg in bus: #while and if msg in socket
        dt = msg.data   # get data in msg
        if msg.arbitration_id < 110 : # check if adressed to the rpi
            #print("data! ")
            #print(count)
            count = count +1
            x = x +1
            arr = arr + "x"
        
            arr = arr + str(msg.arbitration_id)
            arr = arr + "-"
            for index in range(len(dt)) :
                arr = arr + str(dt[index])
                arr = arr + "-"
       
            if x >= 4: #number of sensors to Catch! 
                file = open("/home/pi/Autodrive/testingAutodrive/sensors", "w")
                arr = arr + "q "
                print(arr)
                file.write(arr) # write to file
                file.close()
                arr = "" # reset the string
                x = 0 # reset the count
                



