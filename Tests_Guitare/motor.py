import time
import serial
import base64

motor = serial.Serial('/dev/tty.usbmodem1421', 57600)
#motor = serial.Serial('/dev/tty.usbserial-A9C7FL11', 57600)

count = 0

def write(array, numberOfBytesToReceive = 0):
    motor.write(bytearray(array))
    print("Sent")
    loop=True
    inputWord = ""
    if numberOfBytesToReceive>0:
        while loop:
            numberOfBytesToReceive-=1
            inputByte = base64.b16encode(motor.read())
            inputWord += inputByte
            inputWord += " "
            if numberOfBytesToReceive<0:
                loop=False
            if inputByte == '04':
                loop=False
        return(inputWord)
    else:  
        return True

def start():
    data = [0x01, 0x3f, 0x05, 0x02, 0x00, 0x01, 0x00, 0x00, 0x04]
    print(data)
    write(data)


def homingOff():
    data = [0x01, 0x3f, 0x05, 0x02, 0x00, 0x01, 0x3f, 0x00, 0x04]
    write(data)

def homingOn():
    data = [0x01, 0x3f, 0x05, 0x02, 0x00, 0x01, 0x3f, 0x08, 0x04]
    write(data)


def getCount():
    global count
    count +=1
    return count


def getStatus():
    dataString = "016305020001000004"
    data = base64.b16decode(dataString)
    motor.write(data)
    i=30
    loop=True
    inputWord = ""
    while loop:
        i-=1
        inputByte = base64.b16encode(motor.read())
        inputWord += inputByte
        if i<0:
            loop=False
        if inputByte == '04':
            loop=False
    return inputWord

def getStatus2():
    #data = [0x01, 0x3f, 0x03, 0x02, 0x00, 0x01, 0x04]
    data = [0x01, 0x3f, 0x05, 0x02, 0x00, 0x01, 0x00, 0x00, 0x04]
    write(data)
    i=30
    loop=True
    inputWord = ""
    while loop:
        i-=1
        inputByte = base64.b16encode(motor.read())
        inputWord += inputByte
        if i<0:
            loop=False
        if inputByte == '04':
            loop=False
    print(inputWord)
    n = int(inputWord, 16)
    n = int(n & 0x0000000000000000000000ffffffff00) >> 8
    
    low = n & 0x000000ff
    middlelow = (n & 0x0000ff00) >> 8
    middlehigh = (n & 0x00ff0000) >> 16
    high = (n & 0xff000000) >> 24



def goToPos(n):
    data = [0]*13
    low = n & 0x000000ff
    middlelow = (n & 0x0000ff00) >> 8
    middlehigh = (n & 0x00ff0000) >> 16
    high = (n & 0xff000000) >> 24
    data[0]  =  0x01
    data[1]  =  0x3f
    data[2]  =  0x09
    data[3]  =  0x02
    data[4]  =  0x00
    data[5]  =  0x02
    data[6]  =  getCount()
    data[7]  =  0x02
    data[8]  =  low
    data[9]  =  middlelow
    data[10] =  middlehigh
    data[11] =  high
    data[12] =  0x04
    print(data)
    write(data)


def goToPos2(pos, velo, acc, dec):
    data = [0]*25
    lowp = pos & 0x000000ff
    middlelowp = (pos & 0x0000ff00) >> 8
    middlehighp = (pos & 0x00ff0000) >> 16
    highp = (pos & 0xff000000) >> 24
    lowv = velo & 0x000000ff
    middlelowv = (velo & 0x0000ff00) >> 8
    middlehighv = (velo & 0x00ff0000) >> 16
    highv = (velo & 0xff000000) >> 24
    lowa = acc & 0x000000ff
    middlelowa = (acc & 0x0000ff00) >> 8
    middlehigha = (acc & 0x00ff0000) >> 16
    higha = (acc & 0xff000000) >> 24
    lowd = dec & 0x000000ff
    middlelowd = (pos & 0x0000ff00) >> 8
    middlehighd = (pos & 0x00ff0000) >> 16
    highd = (pos & 0xff000000) >> 24
    data[0] = 0x01
    data[1] = 0x3f
    data[2] = 0x15
    data[3]= 0x02
    data[4] = 0x00
    data[5] = 0x02
    data[6] = getCount()
    data[7] = 0x01
    data[8]=lowp
    data[9]=middlelowp
    data[10]=middlehighp
    data[11]=highp
    data[12]=lowv
    data[13]=middlelowv
    data[14]=middlehighv
    data[15]=highv
    data[16]=lowa
    data[17]=middlelowa
    data[18]=middlehigha
    data[19]=higha
    data[20]=lowd
    data[21]=middlelowd
    data[22]=middlehighd
    data[23]=highd
    data[24] = 0x04
    print ''.join('{:02x}'.format(x) for x in data)
    return write(data, 32)



# goToPos2(sequence[0] * 10000, sequence[1] * 10000, sequence[2] * 10000, sequence[3] * 10000)
# goToPos2(1000000, 1000000, 1000000, 1000000)

# seq->delay(donnee.at(i).at(5) / ((double)k / 5 + 1));

#goToPos(10)


def getReady():
    start()
    time.sleep(0.1)
    homingOn()
    time.sleep(2)
    homingOff()
    print("Ready, Sir")

# def plectre( pos, vit){
#     lowp = pos & 0x00ff
#     highp = (pos & 0xff00) >> 8
#     lowv =  vit & 0x00ff
#     highv =  (vit & 0xff00) >> 8
#     t1 = 255
#     t2 = 255
#     data = [t1, t2, 0x01, 0x07, 0x03, 0x1e, lowp, highp, lowv, highv, 0x00]
#     for ( i=2 i<10  i++){
#         data[10] = data[10]+data[i]
    
#     data[10] = ~data[10]
#     write(QByteArray(data,sizeof(data)),2)








# while 1:
#     serial_line = motor.readline()
#     if(serial_line!=""):
#         print("serie" + serial_line)




