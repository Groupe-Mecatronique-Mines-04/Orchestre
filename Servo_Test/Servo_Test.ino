#include <SoftwareSerial.h>
// Install the ardyno library first !
#include "DynamixelMotor.h"

const uint8_t id=7;
int16_t speed=255;

// Connect the Data wire from the motor board to Pin 8, and connect pin 8 and pin 9 using a 3K resistor
DynamixelInterface &interface=*createSoftSerialInterface(8,9);

DynamixelMotor motor(interface, id);

void setup()
{ 
  delay(100);
  interface.begin(9615);
  delay(100);
  
  motor.init();
  motor.enableTorque();
  
  motor.jointMode();
  motor.speed(speed);
  //delay(5000);
  //motor.wheelMode();
}

void loop() 
{
  delay(200);
  motor.goalPosition(0x21f);
  delay(200);
  motor.goalPosition(0x1ee);
}

