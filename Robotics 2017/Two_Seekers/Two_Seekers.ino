#include <Wire.h>
#include <HTInfraredSeeker.h>

extern "C" { 
#include "utility/twi.h"  // from Wire library, so we can do bus scanning
}


boolean SensorOne = false;
boolean SensorTwo = false;
 
void selectSeeker(uint8_t i) {
  if (i > 7) return;
  Wire.beginTransmission(MULTIPLEXER);
  Wire.write(1 << i);
  Wire.endTransmission();  
}

void setup() {
  while (!Serial);
  delay(500);
  Serial.begin(9600);
  Wire.begin();

  uint8_t data;
  selectSeeker(0);
  if (!twi_writeTo(IR_SENSOR, &data, 0, 1, 1)) {
    SensorOne = true;
    Serial.println("A");
    InfraredSeeker::Initialize();
  }
  selectSeeker(1);
  if (!twi_writeTo(IR_SENSOR, &data, 0, 1, 1)) {
    SensorTwo = true;
    Serial.println("B");
    InfraredSeeker::Initialize();
  }
  delay(500);
  Serial.println("done");
}
  int reading1, strength1;
  int reading2, strength2;




  
  boolean checkLego()  {
  InfraredResult InfraredBall1, InfraredBall2;
  
  if(SensorOne) {
    Serial.println("In 1");
    selectSeeker(0);
    InfraredBall1 = InfraredSeeker::ReadAC();
    reading1 = InfraredBall1.Direction - 5;
    strength1 = InfraredBall1.Strength / 8;
    if(reading1 == -5 || reading1 == 5) {
      reading1 = 0;
      strength1 = 0;
    }
    Serial.println("Done 1");
  }

  if(SensorTwo) {
    Serial.println("In 2");
    selectSeeker(1);
    InfraredBall2 = InfraredSeeker::ReadAC();
    reading2 = InfraredBall2.Direction - 5;
    strength2 = InfraredBall2.Strength / 8;
    if(reading2 == -5 || reading2 == 5) {
      reading2 = 0;
      strength2 = 0;
    }
    Serial.println("Done 2");
  }  
  char buffer[100];
  if(SensorOne && !SensorTwo)
    sprintf(buffer,"%i,%i\n",reading1, strength1);
  if(!SensorOne && SensorTwo)
    sprintf(buffer,"%i,%i\n",reading2, strength2);
  if(SensorOne && SensorTwo)
    sprintf(buffer,"%i,%i,%i,%i\n",reading1, strength1, reading2, strength2);
    // Give status reading that we should move back
  Serial.print(buffer);
}

void loop() {
  checkLego();
}
