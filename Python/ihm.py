from tkinter import *
import tkinter.ttk
import matplotlib
matplotlib.use('TkAgg')
import time
import numpy as np
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.figure import Figure
import matplotlib.pyplot as plt
import threading
import time
from tkinter.filedialog import *
#import midi as midi
import sys
import glob
import serial

encours = True
addressFile = ' '

connectedInstruments = {'guitare': None, 'baguette': None}

musicChoisie = False
instruOk = False
baguetteOk = False

def etatIndicators(root, baguetteIndicator, instruIndicator, musicIndicator):
    if (musicChoisie):
        musicIndicator.config(bg = 'green')
    if (instruOk):
        instruIndicator.config(bg = 'green')
    if (baguetteOk):
        baguetteIndicator.config(bg = 'green')

    root.after(1000, etatIndicators, root, baguetteIndicator, instruIndicator, musicIndicator)

def etatsBoutons(boutonstart, boutonstop):
    if (encours):
        boutonstart.config(state = 'disabled')
        boutonstop.config(state = 'normal')
    else :
        boutonstop.config(state = 'disabled')
        boutonstart.config(state = 'normal')

def buttonOpen(root, music):
    global addressFile
    try :
        addressFile = askopenfilename(filetypes = [("Fichiers MIDI","*.mid"), ("Fichiers textes", "*.txt")])
        file = open(addressFile, 'r')
        #pattern = midi.read_midifile(addressFile)
        #print(pattern)
        texte = addressFile.split('/')[-1]

        music.config(text = "Musique choisie : " + texte)
        global musicChoisie
        musicChoisie = True

    except (FileNotFoundError):
        pass


    #contenu = file.read()
    #print (contenu)
    #file.close()

def buttonStart(boutonstart,boutonstop, root, i = 0, initialTime = None, pointsT = None, pointsY = None, f=None, a = None, canvas=None):
    global encours 
    encours = True
    etatsBoutons(boutonstart, boutonstop)
    updateThread(root, i=0, initialTime=time.time(), pointsT=np.array([0]), pointsY = np.array([0]), f=f, a=a, canvas=canvas)

def buttonStop(boutonstart, boutonstop):
    global encours 
    encours = False
    etatsBoutons(boutonstart, boutonstop)

def chooseInstrument() :
    choisir = Tk()
    choisir.geometry("700x400")
    choisir.title("TestInstruments")
    bouton = Button(choisir, text = "Bouton", command = lambda : push())
    bouton.grid()

def push() :
    print("Ok !")

def rightFrame(root):
    frame = Frame(root, bd=4)

    music = Label(frame, text="Pas de musique choisie")
    music.grid(row = 0, column = 0, columnspan = 2, sticky = N)
    button = Button(frame, text="Open", command = lambda : buttonOpen(root, music))
    button.grid(row = 0, column = 2, sticky = N)

    bottom = Frame(frame, relief = SUNKEN, bd = 4)



    instru = Label(bottom, text = "Instruments :", fg = "Red").grid( row = 1, column = 0, columnspan =2, rowspan = 2)
    pistes = Label(bottom, text = "Pistes :", fg = "Red").grid(row = 1, column = 2, columnspan = 2, rowspan = 2)
    #instru1 = Button(bottom, text = "Choisir instrument", command = lambda : chooseInstrument()).grid( row = 3, column = 1)
    #instru2 = Button(bottom, text = "Choisir instrument", command = lambda : chooseInstrument()).grid( row = 4, column = 1)


    tab_pistes1 = ["Piste 1", "Piste 2"]
    choix_piste1 = StringVar()
    tab_pistes2 = ["Piste 1", "Piste 2"]
    choix_piste2 = StringVar()
    liste1_pistes = ttk.Combobox(bottom, textvariable = choix_piste1, values = tab_pistes1, state = 'readonly').grid(row = 3, column = 2)
    liste2_pistes = ttk.Combobox(bottom, textvariable = choix_piste2, values = tab_pistes2, state = 'readonly').grid(row = 4, column = 2)


    tab_instru1 = ['bonjour']
    choix_instru1 = StringVar()
    tab_instru2 = ['bonjour']
    choix_instru2 = StringVar()


    instru1 = ttk.Combobox(bottom, textvariable = choix_instru1, values = tab_instru1, state = 'readonly')
    instru2 = ttk.Combobox(bottom, textvariable = choix_instru2, values = tab_instru2, state = 'readonly')
    instru1.grid( row = 3, column = 1)
    instru2.grid( row = 4, column = 1)

    maj = Button(bottom, text = "Actualiser", command = lambda : updateInstruments(instru1, instru2))  
    maj.grid( row = 5, column = 1)

    bottom.grid(row = 1, columnspan = 3,rowspan = 2, sticky = E)

    return frame
'''
def choixPiste (var,root, choix = None) :
	if var.get() == 1 :
		choix = 1
	if var.get() == 2 : 
		choix = 2

	root.after(100, choixPiste, var, root)
'''




def leftFrame(root):
    frame = Frame(root)

    top = Frame(frame, relief=SUNKEN, bd=4)
    ###
    f = Figure(figsize=(3,2), dpi=100)
    canvas = FigureCanvasTkAgg(f, master=top)
    canvas.show()
    canvas.get_tk_widget().grid(row = 0)


   

    bottom = Frame(frame, relief=SUNKEN, bd=4)

    baguetteLabel = Label(bottom, text="Baguette")
    baguetteIndicator = Label(bottom, text="", bg='red', relief=RIDGE, width=1)
    baguetteIndicator.grid(row=0, column=0)
    baguetteLabel.grid(row=0, column=1, columnspan = 2)

    instruLabel = Label(bottom, text = "Instruments")
    instruIndicator = Label(bottom, text="", bg="red", relief=RIDGE, width=1)
    instruIndicator.grid(row=1,column=0)
    instruLabel.grid(row=1,column=1, columnspan = 2)

    musicLabel = Label(bottom, text = "Musique")
    musicIndicator = Label(bottom, text="", bg="red", relief=RIDGE, width=1)
    musicIndicator.grid(row=2,column=0)
    musicLabel.grid(row=2,column=1, columnspan = 2)

    etatIndicators(bottom, baguetteIndicator, instruIndicator, musicIndicator)

    
    top.grid(row = 0, sticky = N+S , pady = 0)
    bottom.grid(row = 1, sticky = S , pady = 0)

    a = f.add_subplot(111)
    a.set_frame_on(False)
    a.axes.get_xaxis().set_visible(False)
    a.axes.get_yaxis().set_visible(False)
    #a.set_title("")

    stop = Button(top, text="Stop", state = 'disabled', command = lambda : buttonStop(start, stop) )
    stop.grid(row = 2, pady=15, sticky = S)

    start = Button(top, text="Start", command = lambda : buttonStart(start,stop, root, i=0, initialTime=time.time(), pointsT=np.array([0]), pointsY = np.array([0]), f=f, a=a, canvas=canvas), )
    start.grid(row=1, pady=15, sticky = S)
    
   
    return frame


def updateThread(root, i = 0, initialTime = None, pointsT = None, pointsY = None, f=None, a = None, canvas=None):

    #print("Call number {}".format(i))
    if (encours):
        t = time.time()-initialTime
        newY = np.sin(2*np.pi*t/2)
        toRemove = []
        for j in range(len(pointsT)):
            if pointsT[j] < t - 10:
                toRemove += [j]
        
        for j in toRemove:
            pointsT = np.delete(pointsT, j)
            pointsY = np.delete(pointsY, j)
        pointsT = np.append(pointsT, t)
        pointsY = np.append(pointsY,newY)
        a.clear()
        a.set_xlim(np.max(pointsT) - 5, np.max(pointsT) + 5)
        a.set_ylim(np.min(pointsY), np.max(pointsY))
        a.plot(pointsT, pointsY)
        canvas.draw()

        root.after(10, updateThread, root, i+1, initialTime, pointsT, pointsY, f, a, canvas)

def main():
    root = Tk()
    root.geometry("700x400")
    left = leftFrame(root)
    right = rightFrame(root)
    left.grid(row = 0, column=0 ,sticky = NS)
    right.grid(row = 0, column=1,sticky=NS)
    root.title("Test")
    root.mainloop()



##Serial


def serial_ports():
    """ Lists serial port names

        :raises EnvironmentError:
            On unsupported or unknown platforms
        :returns:
            A list of the serial ports available on the system
    """
    if sys.platform.startswith('win'):
        ports = ['COM%s' % (i + 1) for i in range(256)]
    elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
        # this excludes your current terminal "/dev/tty"
        ports = glob.glob('/dev/tty[A-Za-z]*')
    elif sys.platform.startswith('darwin'):
        ports = glob.glob('/dev/tty.*')
    else:
        raise EnvironmentError('Unsupported platform')

    result = []
    for port in ports:
        try:
            s = serial.Serial(port)
            s.close()
            result.append(port)
        except (OSError, serial.SerialException):
            pass
    return result

def connectInstruments():
    ports = serial_ports()
    for port in ports:
        if('usb' not in port):
            ports.remove(port)
    print(ports)
    for p in ports :
        ser = serial.Serial(p, 19200)
        #0xF0 : System Exclusive
        #0x04 : Group 04 Mecatro
        #Ox7F : EOF
        values = bytearray([0xf0, 0x04, 0x7f])
        ser.write(values)



def updateInstruments(instru1, instru2):
   
    l = serial_ports()
    instru1.config(values = l)
    instru2.config(values = l)
    instru1.config(values= ["Guitare", "Flûte"])
    instru2.config(values= ["Guitare", "Flûte"])


if __name__ == '__main__':
    print(serial_ports())
    main()  