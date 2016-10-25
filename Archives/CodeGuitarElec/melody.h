#ifndef MELODY_H
#define MELODY_H

#include "MIDI/Binasc.h"
#include "MIDI/MidiEvent.h"
#include "MIDI/MidiEventList.h"
#include "MIDI/MidiFile.h"
#include "MIDI/MidiMessage.h"
#include "MIDI/Options.h"
#include <QString>

class Melody
{
public:
    Melody();
    ~Melody();

public:
    double tick;
    double duration;
    int    pitch;
    int velocity;

    // function declarations:
    static int       notecompare       (const void* a, const void* b);

};



#endif // MELODY_H


