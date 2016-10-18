#include <MIDI.h>


// Create default 'Midi' instance
// Uses Serial port (which ones ?)
MIDI_CREATE_DEFAULT_INSTANCE();

boolean playing = false;
void setup() {
  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNotOff(handleNoteOff);
  MIDI.begin(1); // Launch MIDI and listen to channel 1
  initializeLinmot(); // Jeremy's getReady() function to initialize the linear motor
}

void loop() {
  // Automatically calls handleNoteOn and handleNoteOff
  // when a message is received
  MIDI.read();
}

// Note pressed, channel is always 1 here
// pitch from 0 to 127, math relation with the frequency
// velocity is like the volume (we don't care)
void handleNoteOn(byte channel, byte pitch, byte velocity)
{
  if(playing == true){
    handleNoteOff(channel, pitch, velocity);
  }
  playing = true;

  int pos = getRightPos();
  sendLinmotToPos(pos);
}

// Note released, channel is always 1 here
// pitch from 0 to 127, math relation with the frequency
// velocity is like the volume (we don't care)
void handleNoteOff(byte channel, byte pitch, byte velocity)
{
  playing = false;
}

// TODO : Math formula converting pitch to frequency to the linmot position
int getRightPos(byte pitch){
  return 0;
}

void initializeLinmot(){
  
}

void sendLinmotToPos(pos){
  
}

