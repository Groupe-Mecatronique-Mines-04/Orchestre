import rtmidi_python as rtmidi
from comserie import Serie
import serial
midi_in = rtmidi.MidiIn(b'')
midi_in.open_port(1)


s = Serie(57600)
portSerie = ""
for key in s.ports:
    portSerie = s.ports.get(key)

print(s.ports)
print("Connection à la carte %s" % portSerie)
arduino = serial.Serial(portSerie, 57600)
print("Connected")

while True:
    message, delta_time = midi_in.get_message()
    if message:
        r = message[0] << 16 | message[1] << 8 | message[2]
        print(message[1])
        arduino.write(bytearray([message[0], message[1], message[2]]))
