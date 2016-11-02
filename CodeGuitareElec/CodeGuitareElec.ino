// Serial1 is the LinMot

int LINMOT_BAUDRATE = 57600;
int count = 0;

int getCount(){
    count = (count+1)%15;
    return count;
}

int getSerialInteger(){
    int serialdata = 0;
    byte inbyte = '0';
    boolean isNegative = false;

    while (inbyte != ';')
    {
      inbyte = Serial.read();
      if (inbyte > 0 && inbyte != ';')
      {
        if(inbyte == '-'){isNegative = true;}
        else{serialdata = serialdata * 10 + inbyte - '0';}
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
	Serial.print("Sending : ");
	Serial.write((uint8_t*)buffer, length);
	Serial1.write((uint8_t*)buffer, length);
}

int linMotWriteWithAnswer(int* buffer, int length){
	Serial.print("Sending : ");
	Serial.write((uint8_t*)buffer, length);
	Serial1.write((uint8_t*)buffer, length);
	int receivedAnswer = 0;
  while(Serial1.available()) {
    receivedAnswer = receivedAnswer & Serial1.read();
    receivedAnswer = receivedAnswer << 1;
  }
  return receivedAnswer;
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
  delay(100);
  linMotHomingOn();
  delay(1000);
  linMotHomingOff();
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
  linMotGoToPos(pos, 1000000, 1000000, 1000000);
}




void setup(){
    Serial1.begin(LINMOT_BAUDRATE);
    Serial.begin(115200);

    linMotStartProcedure();
    delay(5000);

}

void loop(){
 if (Serial.available()) {
    char ch = Serial.read();
    if(ch=='X'){
      long pos = getSerialInteger();
      linMotGoToPos(pos);
    }
  } 
}