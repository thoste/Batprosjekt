import time
import serial as ser

com = ser.Serial('/dev/cu.usbserial-A9M5LZZ7', baudrate=9600)

com.isOpen()

print 'Enter your commands below.\r\nInsert "exit" to leave the application.'

input=1
while 1 :
    # get keyboard input
    input = raw_input(">> ")
        # Python 3 users
        # input = input(">> ")
    if input == 'exit':
        com.close()  
        exit()
    else:
        # send the character to the device
        # (note that I happend a \r\n carriage return and line feed to the characters - this is requested by my device)
        com.write(input + '\r\n')
        out = ''
        # let's wait one second before reading output (let's give device time to answer)
        time.sleep(1)
        while com.inWaiting() > 0:
            out += com.read(1)

        if out != '':
            print ">>" + out