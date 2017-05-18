#Bibliotheque python-midi, branch python3
import midi
import time
import serial
import datetime
from comserie import Serie
import wii
import random
import sequencer_osx


def wait_until(execute_it_now):
    while True:
        diff = (execute_it_now - datetime.datetime.now()).total_seconds()
        if diff <= 0:
            return
        elif diff <= 0.01:
            time.sleep(0.0001)
        elif diff <= 0.1:
            time.sleep(0.001)
        elif diff <= 0.5:
            time.sleep(0.01)
        elif diff <= 1.5:
            time.sleep(0.1)
        else:
            time.sleep(1)

s = Serie(57600)

portSerie = ""
for key in s.ports:
    portSerie = s.ports.get(key)

print("Connection à la carte %s" % portSerie)
arduino = serial.Serial(portSerie, 57600)
bpm = 92. #tempo
isConnected = False

def event(evt):
    #Wiimote found
    if(evt[0] == 0):
        print("Wiimote trouvée")
    #Wiimote connected
    elif(evt[0] == 1):
        print("Wiimote connectée")
        global isConnected
        isConnected = True
    #Connection issue (L2CAP)
    elif(evt[0] == 2):
        print("La connection a échoué, veuillez réessayer (L2CAP)")
        exit(0)
    elif(evt[0] == 3):
        print("La connection a échoué, veuillez réessayer")
        exit(0)
    #New frequency value
    elif(evt[0] == 4):
        global bpm
        bpm = 60*evt[1]
        print("bpm = %.3fBPM" % bpm)

thread = wii.WiiUse("/Users/florianveaux/Desktop/wiiuseexample", event)
thread.start()
while not isConnected:
    pass


pat=midi.read_midifile("oalj.mid")
pat.make_ticks_abs()
events = []
for track in pat:
    for event in track:
        if (isinstance(event, midi.events.NoteOnEvent) or isinstance(event, midi.events.NoteEvent)) and event.channel == 0:
            events.append(event)

events.sort(key=lambda x: x.tick)
max_tick = events[-1].tick + 100
tick = 0
pos = 0
time.sleep(1)
lastTime = datetime.datetime.now()

while pos < len(events):
    if isinstance(events[pos], midi.events.NoteEvent):
        nbTick = 0
        if pos == 0:
            nbTick = events[pos].tick
        else:
            nbTick = events[pos].tick - events[pos-1].tick

        if pos == 10:
            bpm = 120.
        wait_until(lastTime + datetime.timedelta(0, 0, nbTick * 60000000/(bpm*960)))
        lastTime = datetime.datetime.now()
        print(events[pos].data[0])
        chan = events[pos].channel
        status = 0b10000000 | (chan)
        if isinstance(events[pos], midi.events.NoteOnEvent):
            status |= 0b00010000
        if isinstance(events[pos], midi.events.NoteOffEvent):
            status |= 0b00000000
        data1 = events[pos].data[0] + 10
        data2 = events[pos].data[1]
        r = status << 16 | data1 << 8 | data2
        print(bin(r))
        arduino.write(bytearray([status, data1, data2]))
    pos += 1;
arduino.close()
