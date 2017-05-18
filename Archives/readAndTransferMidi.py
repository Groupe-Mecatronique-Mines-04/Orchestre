import midi
import time
import serial
arduino = serial.Serial('/dev/cu.usbmodemFA131', 57600)
f = 60. #tempo

pat=midi.read_midifile("auclair.mid")
pat.make_ticks_abs()
events = []
for track in pat:
    for event in track:
        if isinstance(event, midi.events.NoteOnEvent) and event.channel == 1:
            events.append(event)

events.sort()
max_tick = events[-1].tick + 100
tick = 0
pos = 0
time.sleep(1)
for tick in range(0, max_tick):
    while pos < len(events) and events[pos].tick == tick:
        chan = events[pos].channel
        status = 0b10000000 | chan
        if isinstance(events[pos], midi.events.NoteOnEvent):
            status |= 0b00010000
        data1 = events[pos].data[0]
        data2 = events[pos].data[1]
        r = status << 16 | data1 << 8 | data2
        print bin(r)
        arduino.write(bytearray([0x4d, status, data1, data2]))
        pos += 1
    time.sleep(60./100/256)
