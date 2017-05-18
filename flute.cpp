#include "mbed.h"
DigitalOut fan(p5);
DigitalOut solen(p6);
DigitalOut test(LED3);
Serial pc(USBTX, USBRX, 57600);
Serial moteur(p13, p14, 57600);
int count = 0;

char* buffer = new char[16];
int* posForPitchUp = new int[128];
int* posForPitchDown = new int[128];
int lastPitch = 0;

int channel = 1; //1->16

char noteOnHex = 0x90 + channel - 1;
char noteOffHex = 0x80 + channel - 1;

bool isPlaying = false;
bool isListeningToMidi = true;
int getCount(){
    count = (count+1)%15;
    return count;
}

long getSerialInteger(){
    long serialdata = 0;
    char inbyte = '0';
    bool isNegative = false;

    while (inbyte != ';')
    {
      inbyte = pc.getc();
      if(    inbyte == '0'
          || inbyte == '1'
          || inbyte == '2'
          || inbyte == '3'
          || inbyte == '4'
          || inbyte == '5'
          || inbyte == '6'
          || inbyte == '7'
          || inbyte == '8'
          || inbyte == '9'
          || inbyte == ';'
          || inbyte == '-'
          ){
        if (inbyte > 0 && inbyte != ';')
        {
          if(inbyte == '-'){isNegative = true;}
          else{serialdata = serialdata * 10 + inbyte - '0';}
        }
      }
    }
    inbyte = 0;
    if(isNegative){
      return -serialdata;
    }
    else{
      return serialdata;
    }
}


void linMotSimpleWrite(int* buffer, int length){
    pc.printf("Sending : ");
    for(int i=0; i<length;i++){
        if(buffer[i]< 0x0f){
            pc.printf("%x", 0x0);
        }
        pc.printf("%x", buffer[i]);
        pc.printf(" ");
        moteur.putc(buffer[i]);
    }
    pc.printf("\n");

}

void linMotStart(){
    int data[] = {0x01, 0x3f, 0x05, 0x02, 0x00, 0x01, 0x00, 0x00, 0x04};
    linMotSimpleWrite(data, 9);
}
void linMotHomingOn(){
    int data[] = {0x01, 0x3f, 0x05, 0x02, 0x00, 0x01, 0x3f, 0x08, 0x04};
    linMotSimpleWrite(data, 9);
}

void linMotHomingOff(){
    int data[] = {0x01, 0x3f, 0x05, 0x02, 0x00, 0x01, 0x3f, 0x00, 0x04};
    linMotSimpleWrite(data, 9);
}

void linMotStartProcedure(){
  linMotStart();
  wait_ms(100);
  linMotHomingOn();
  wait_ms(5000);
  linMotHomingOff();
  pc.printf("Homing Off\n");
}


void linMotGoToPos(long pos, long velo, long acc, long dec)
{
    int data[25];

    int lowp = pos & 0x000000ff;
    int middlelowp = (pos & 0x0000ff00) >> 8;
    int middlehighp = (pos & 0x00ff0000) >> 16;
    int highp = (pos & 0xff000000) >> 24;

    int lowv = velo & 0x000000ff;
    int middlelowv = (velo & 0x0000ff00) >> 8;
    int middlehighv = (velo & 0x00ff0000) >> 16;
    int highv = (velo & 0xff000000) >> 24;

    int lowa = acc & 0x000000ff;
    int middlelowa = (acc & 0x0000ff00) >> 8;
    int middlehigha = (acc & 0x00ff0000) >> 16;
    int higha = (acc & 0xff000000) >> 24;

    int lowd = dec & 0x000000ff;
    int middlelowd = (pos & 0x0000ff00) >> 8;
    int middlehighd = (pos & 0x00ff0000) >> 16;
    int highd = (pos & 0xff000000) >> 24;

    data[0] = 0x01;
    data[1] = 0x3f;
    data[2] = 0x15; //Length
    data[3]= 0x02;
    data[4] = 0x00; //Command ID
    data[5] = 0x02;
    data[6] = getCount(); // Message Sub ID - Count
    data[7] = 0x01; //Message Main ID
    data[8]=lowp;
    data[9]=middlelowp;
    data[10]=middlehighp;
    data[11]=highp;
    data[12]=lowv;
    data[13]=middlelowv;
    data[14]=middlehighv;
    data[15]=highv;
    data[16]=lowa;
    data[17]=middlelowa;
    data[18]=middlehigha;
    data[19]=higha;
    data[20]=lowd;
    data[21]=middlelowd;
    data[22]=middlehighd;
    data[23]=highd;
    data[24] = 0x04; //End
    linMotSimpleWrite(data, 25);
}


void linMotGoToPos(long pos){
  linMotGoToPos(pos, 3000000, 1000000, 1000000);
}

void linMotGoToPosMM(long posInMM){
  if(posInMM >= 0 && posInMM <= 120){
    linMotGoToPos(posInMM*10000, 3000000, 1000000, 1000000);
  }
}

void setPlaying(bool flag){
  isPlaying = flag;
  if(isPlaying){
    solen = 0;
  } else{
    solen = 1;
  }
}

void loop(){
  if (pc.readable() and !isListeningToMidi) {
    char ch = pc.getc();
    if(ch=='?'){
      pc.printf("Manual : \n");
      pc.printf("A : Check if uC is still alive\n");
      pc.printf("S : Start the motor and rock. Mandatory before X\n");
      pc.printf("X100; Go to 100mm. ';' is important !\n");
    }
    if(ch=='A'){
      pc.printf("I'm alive !\n");
    }
    if(ch=='P'){
      long pos = getSerialInteger();
      pc.printf("Playing pitch %d\n", pos);
      linMotGoToPosMM(posForPitchUp[pos]);
      pc.printf("Sent\n");
    }
    if(ch=='X'){
      long pos = getSerialInteger();
      pc.printf("Going to %d\n", pos);
      linMotGoToPosMM(pos);
      pc.printf("Sent\n");
    }
    if(ch=='S'){
      pc.printf("Starting the motor...\n");
      linMotStartProcedure();
      pc.printf("Motor Ready\n");
    }
    if(ch=='O'){
      linMotGoToPosMM(88);
      wait_ms(500);
      linMotGoToPosMM(68);
      wait_ms(500);
      linMotGoToPosMM(52);
      wait_ms(500);
      linMotGoToPosMM(44);
      wait_ms(500);
      linMotGoToPosMM(28);
      wait_ms(500);
      linMotGoToPosMM(44);
      wait_ms(500);
      linMotGoToPosMM(52);
      wait_ms(500);
      linMotGoToPosMM(68);
      wait_ms(500);
      linMotGoToPosMM(88);
      wait_ms(500);
    }
    if(ch=='M'){
      isListeningToMidi = true;
      pc.printf("Listening to Midi now");
    }
    
  } else if(isListeningToMidi){
    buffer = new char[16];
    buffer[0] = pc.getc();
    //NoteOnEvent
    if(buffer[0] == noteOnHex){
      buffer[1] = pc.getc();
      buffer[2] = pc.getc();
      //Get pitch
      char pitch = buffer[1];
      if(posForPitchDown[pitch] == -1){
          setPlaying(false);
      }
      else if(lastPitch > pitch){
        linMotGoToPosMM(posForPitchDown[pitch]);
      } else {
        linMotGoToPosMM(posForPitchUp[pitch]);
      }
      pc.printf("Played a note");
      test = !test;
      setPlaying(true);
    }
    //NoteOffEvent
    else if(buffer[0] == noteOffHex){
      buffer[1] = pc.getc();
      buffer[2] = pc.getc();
      if(buffer[1] == lastPitch){
        setPlaying(false);
      }
    }
    //SystemExclusive
    else if(buffer[0] == 0xF0){
        buffer[1] = pc.getc();
        buffer[2] = pc.getc();
        if(buffer[1] == 0x04 and buffer[2] == 0x7F){
            pc.putc(0xF0); //System Exclusive
            pc.putc(0x04); //Groupe 04, handshake id
            pc.putc(0x46); //'F'
            pc.putc(0x6c); //'l'
            pc.putc(0x75); //'u'
            pc.putc(0x74); //'t'
            pc.putc(0x65); //'e'
            pc.putc(0x7F); //'EOF'
        }
    }
    else {
      isListeningToMidi = false;
      pc.printf("Not listening anymore");
    }
  }

}

int main() {
    //Guitare
    /*
     * 
     */
    //Flute
    for(int i = 0; i < 128; i++){
      posForPitchUp[i] = -1;
      posForPitchDown[i] = -1;
    }
    posForPitchUp[69] = 120;
    posForPitchUp[70] = 106;
    posForPitchUp[71] = 94;
    posForPitchUp[72] = 84;
    posForPitchUp[73] = 74;
    posForPitchUp[74] = 64;
    posForPitchUp[75] = 55;
    posForPitchUp[76] = 47;
    posForPitchUp[77] = 38;
    posForPitchUp[78] = 30;
    posForPitchUp[79] = 22;
    posForPitchUp[80] = 14;
    posForPitchDown[69] = 120;
    posForPitchDown[70] = 108;
    posForPitchDown[71] = 97;
    posForPitchDown[72] = 86;
    posForPitchDown[73] = 76;
    posForPitchDown[74] = 67;
    posForPitchDown[75] = 58;
    posForPitchDown[76] = 49;
    posForPitchDown[77] = 41;
    posForPitchDown[78] = 33;
    posForPitchDown[79] = 25;
    posForPitchDown[80] = 17;

    setPlaying(true);
    fan = 1;
    while(1){
        loop();
    }
}