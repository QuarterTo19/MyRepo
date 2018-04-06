File = open("KeySender.txt", "r+")
File2 = open("KeyHexed.txt", "w+")

counter = 0
revcount = 7
value  = 0
while True:
    char = File.read(1)
    if not char :
        break
    elif char:
        try:
            a = int(char[0])
            if counter % 8 == 0:
                if value != 0:
                    value = hex(value)[2:].split('x')[-1].zfill(2)
                    File2.write(value)

                counter = 0
                revcount = 7
                value = 0

            if a == 1:
                value += pow(2, revcount)
                revcount -=1
            elif a == 0:
                revcount -= 1

            counter += 1
        except ValueError:
            break

File.close()
File2.close()
