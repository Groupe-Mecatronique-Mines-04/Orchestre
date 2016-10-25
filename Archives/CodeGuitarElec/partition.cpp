#include "partition.h"

Partition::Partition()
{

}

Partition::~Partition()
{

}

void Partition::convertToMelody(MidiFile& midifile, vector<Melody>& melody, int track) {
  donnee.clear();
    midifile.absoluteTicks();
  if (track < 0 || track >= midifile.getNumTracks()) {
      return;
   }
   int numEvents = midifile.getNumEvents(track);
   vector<int> state(128);   // for keeping track of the note states
   int i;
   for (i=0; i<128; i++) {
      state[i] = -1;
   }

   melody.reserve(numEvents);
   melody.clear();

   Melody mtemp;
   int command;
   int pitch;
   int vel;

   for (i=0; i<numEvents; i++) {
      command = midifile[track][i][0] & 0xf0;
      if (command == 0x90) {
         pitch = midifile[track][i][1];
         vel = midifile[track][i][2];
            // note on
            state[pitch] = midifile[track][i].tick;

      }
      else if (command == 0x80) {
         // note off
         pitch = midifile[track][i][1];
         if (state[pitch] == -1) {
            continue;
         }
         mtemp.tick = state[pitch];
         mtemp.duration = midifile[track][i].tick - state[pitch];
         mtemp.pitch = pitch;
         mtemp.velocity = vel;
         melody.push_back(mtemp);

       QVector<double> instruction;
       instruction.append(mtemp.tick);
       instruction.append(mtemp.pitch);
       instruction.append(mtemp.duration);
       instruction.append(mtemp.velocity);
       donnee.append(instruction);

         state[pitch] = -1;
      }
   }
}

void Partition::sortMelody(vector<Melody>& melody) {
   qsort(melody.data(), melody.size(), sizeof(Melody), Melody::notecompare);
}


void Partition::load(QString path, int track) {
   MidiFile midifile(path.toStdString());
   vector<Melody> melody;
   convertToMelody(midifile, melody,track);
   sortMelody(melody);
}

void Partition::print(){
    if (donnee.size()>0) {
    for  (int i=0 ; i<donnee.size()-1 ; i++) {
        cout << "Start : " <<donnee.at(i).at(0) << " Note : " <<donnee.at(i).at(1) << " Duration : " <<donnee.at(i).at(2) << " Velocity : " << donnee.at(i).at(3) << endl;
    }
    }
}

