// Serial1 is the LinMot

int LINMOT_BAUDRATE = 57600;
int USB_SERIAL_BAUDRATE = 57600;
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
    for(int i=0; i<length;i++){
        if(buffer[i]< 0x0f){
            Serial.print(0x0, HEX);
        }
        Serial.print(buffer[i], HEX);
        Serial.print(' ');
        Serial1.write(buffer[i]);
        delay(1);
    }
     Serial.println(' ');
	//Serial.write((uint8_t*)buffer, length);

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
    // int data[] = {0x0, 0x1, 0x3, 0xf, 0x0, 0x5, 0x0, 0x2, 0x0, 0x0, 0x0, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4};
    linMotSimpleWrite(data, 9);
}
void linMotHomingOn(){
    int data[] = {0x01, 0x3f, 0x05, 0x02, 0x00, 0x01, 0x3f, 0x08, 0x04};
    // int data[] = {0x0, 0x1, 0x3, 0xf, 0x0, 0x5, 0x0, 0x2, 0x0, 0x0, 0x0, 0x1, 0x3, 0xf, 0x0, 0x8, 0x0, 0x4};
    linMotSimpleWrite(data, 9);
}

void linMotHomingOff(){
    int data[] = {0x01, 0x3f, 0x05, 0x02, 0x00, 0x01, 0x3f, 0x00, 0x04};
    // int data[] = {0x0, 0x1, 0x3, 0xf, 0x0, 0x5, 0x0, 0x2, 0x0, 0x0, 0x0, 0x1, 0x3, 0xf, 0x0, 0x0, 0x0, 0x4};
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
  linMotGoToPos(pos, 3000000, 1000000, 1000000);
}




void setup(){
    //Serial1.begin(LINMOT_BAUDRATE);
    //Serial.begin(USB_SERIAL_BAUDRATE);

    Serial1.begin(57600);
    Serial.begin(57600);

    // linMotStartProcedure();
    // delay(5000);

}

void loop(){
 if (Serial.available()) {
    char ch = Serial.read();
    if(ch=='X'){
      long pos = 0;
      Serial.print("Going to ");
      Serial.println(pos);
      linMotGoToPos(pos);
      Serial.println("Sent");
    }
    if(ch=='Y'){
      long pos = 500000;
      Serial.print("Going to ");
      Serial.println(pos);
      linMotGoToPos(pos);
      Serial.println("Sent");
    }
    if(ch=='Z'){
      long pos = 1000000;
      Serial.print("Going to ");
      Serial.println(pos);
      linMotGoToPos(pos);
      Serial.println("Sent");
    }
    if(ch=='W'){
      long pos = 2000000;
      Serial.print("Going to ");
      Serial.println(pos);
      linMotGoToPos(pos);
      Serial.println("Sent");
    }
    if(ch=='S'){
      Serial.println("Starting the motor...");
      linMotStartProcedure();
      Serial.println("Motor Ready");
    }
  } 



  // if (Serial1.available()) {
  //   int inByte = Serial1.read();
  //   Serial.write(inByte);
  // }

  // // read from port 0, send to port 1:
  // if (Serial.available()) {
  //   int inByte = Serial.read();
  //   Serial1.write(inByte);
  // }

}