import serial
ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=1)

print ser.portstr       #check which port was really used

var = 1

while (var == 1):
    tmp = ser.readline()      #write a string
    if (tmp == "" or tmp == " "):
        tmp = "balls"
    else:
        print tmp
    #othertmp = raw_input("Enter q to quit program")
    #if(othertmp == "q"):
#       break
    #else:
    #   print tmp

ser.close()             #
