import serial
import sys
import glob
import time
class Serie():

    def __init__(self, baudrate):
        self.baudrate = baudrate
        self.ports = {} #{"/dev/tty.usb******", "Guitare"}
        self.getPorts(update=True)

    def getPorts(self, update = False):
        if update == False:
            return self.ports

        if sys.platform.startswith('win'):
            ports = ['COM%s' % (i + 1) for i in range(256)]
        elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
            # this excludes your current terminal "/dev/tty"
            ports = glob.glob('/dev/tty[A-Za-z]*')
        elif sys.platform.startswith('darwin'):
            ports = glob.glob('/dev/tty.*')
        else:
            raise EnvironmentError('Unsupported platform')

        result = {}
        toRemove = []
        for port in ports:
            if 'usb' not in port:
                toRemove += [port]
        for p in toRemove:
            ports.remove(p)
        for port in ports:
            try:
                s = serial.Serial(port, self.baudrate, timeout=0.05)
                values = bytearray([0xf0, 0x04, 0x7f])
                s.write(values)
                string = s.read(size=20)
                s.close()
                if(len(string) > 3 and string[0] == 0xf0 and string[1] == 0x04 and string[-1] == 0x7f):
                    result[string[2:-1].decode("utf-8")] = port
            except (OSError, serial.SerialException):
                pass

        self.ports = result
        return self.ports

    def checkIfAlive(self, portName):
        port = self.ports.get(portName)
        if port != None:
            try:
                s = serial.Serial(port, self.baudrate, timeout=0.05)
                values = bytearray([0xf0, 0x04, 0x7f])
                s.write(values)
                string = s.read(size=20)
                s.close()
                if(len(string) > 3 and string[0] == 0xf0 and string[1] == 0x04 and string[-1] == 0x7f):
                    if portName == string[2:-1].decode("utf-8"):
                        return True
            except (OSError, serial.SerialException):
                pass
            self.ports.pop(portName)

        return False

s = Serie(57600)
print(s.ports)
#print(s.checkIfAlive("Flute"))
