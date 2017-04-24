#include <Wire.h>

int index;
int strength;
byte error;

extern "C" { 
#include "utility/twi.h"  // from Wire library, so we can do bus scanning
}

#define SENSOR 0x50

void setup() {
  Serial.begin(9600);
  Wire.begin();

  Wire.beginTransmission(SENSOR);
  error = Wire.endTransmission();

  if (error != 0) {
    Serial.print("Couldn't find the sensor");
    while(1);
  }
}

void loop() {
  Wire.requestFrom(SENSOR, 2);
  if (Wire.available()) {
    index = Wire.read();
    strength = Wire.read();
    char buffer[100];
    sprintf(buffer,"%i,%i\n",index, strength);
    Serial.print(buffer);
  }
}
