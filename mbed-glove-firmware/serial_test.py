import serial
import serial.tools.list_ports


for port in serial.tools.list_ports.comports():
    #print port
    if ('JLink' in str(port)):
        break
        
print port

ser = serial.Serial(str(port)[:4])


print ser.read(ser.write('Working'))






