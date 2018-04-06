#read file
File = open("bitstosend.txt", "r+")
File2 = open("ArduinoKey.txt", "r+")
File3 = open("KeySender.txt", "w+")
#File = open("keydata.txt", "r")
#Xor each bit with key from a5/1

#hex(15)[2:].zfill(2)
'''
key = File2.read()
#print key
key = key.replace("\r\n", "")
#print key
File2.close()
File2 = open("ArduinoKey.txt", "w+")
File2.write(key)
File2.close()
'''
counter = 0
startcount = 0
revcount = 7
value = 0
value2 = 0
countb = 0
#reads the bits LbL
#but firstly do the negation
while True:
    char = File.read(1)
    char2 = File2.read(1)
    if not char :
        break
    elif char:
        try:
            a = int(char[0])
            b = int(char2[0])
            a = a ^ b
            File3.write(str(b))
            if counter % 8 == 0:
                if value != 0:
                    value = hex(value)[2:].split('x')[-1].zfill(2)
                    value2 = hex(value2)[2:].split('x')[-1].zfill(2)
                    #reverse values here
                    print "irsend.sendNec(0x8a75"+value+value2+", 32);"
                    print "delay(500);"

                counter = 0
                startcount = 0
                revcount = 7
                value2 = 0
                value = 0

            if a == 1:
                value += pow(2, startcount)
                startcount += 1
                revcount -= 1
            elif a == 0:
                value2 += pow(2, startcount)
                startcount += 1
                revcount -= 1
            counter = counter + 1
        except ValueError:
            break
File.close()
File2.close()
File3.close()
