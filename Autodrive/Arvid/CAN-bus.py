import can

bus = can.interface.Bus(channel='can0', bustype='socketcan_native')
#mess = can.Message(arbitration_id=200, data=[1, 25, 0, 1, 3, 1, 4, 1])
#bus.send(mess)

#notifier = can.Notifier(bus, [can.Printer()])

#msg = can.BufferedReader.get_message(0.2)

#file = open("sensors", "w")        
#file.write("TEST")
#file.close()


while(1):
    print("recieving...")
    arr = ""
    x = 0
    for msg in bus:
        dt = msg.data
        if msg.arbitration_id < 110 :
            print("data!")
            x = x +1
            arr = arr + "x"
        
            arr = arr + str(msg.arbitration_id)
            arr = arr + "-"
            for index in range(len(dt)) :
                arr = arr + str(dt[index])
                arr = arr + "-"
       
            if x >= 4: 
                file = open("sensors", "w")
                arr = arr + "q "
                print(arr)
                file.write(arr)
                file.close()
                arr = ""
                x = 0
                
            
    


