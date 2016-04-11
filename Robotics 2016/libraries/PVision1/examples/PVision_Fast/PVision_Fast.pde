// Example of using the PVision library for interaction with the Pixart sensor on a WiiMote
// This work was derived from Kako's excellent Japanese website
// http://www.kako.com/neta/2007-001/2007-001.html

// Steve Hobley 2009 - www.stephenhobley.com

#include <I2C.h>
#include <PVision_Fast.h>

PVision_Fast ircam;
byte result;

void setup() {
  Serial.begin(9600);
  ircam.init();
}

void loop() {
   
  result = ircam.read();
  
  if (result & BLOB1) {
    Serial.print("B1 X:");
    Serial.print(ircam.Blob1.X);
    Serial.print(" Y:");
    Serial.print(ircam.Blob1.Y);
    Serial.print(" S:");
    Serial.println(ircam.Blob1.Size);
  }
  
  if (result & BLOB2) {
    Serial.print("B2 X:");
    Serial.print(ircam.Blob2.X);
    Serial.print(" Y:");
    Serial.print(ircam.Blob2.Y);
    Serial.print(" S:");
    Serial.println(ircam.Blob2.Size);
  }
  if (result & BLOB3) {
    Serial.print("B3 X:");
    Serial.print(ircam.Blob3.X);
    Serial.print(" Y:");
    Serial.print(ircam.Blob3.Y);
    Serial.print(" S:");
    Serial.println(ircam.Blob3.Size);
  }
  if (result & BLOB4) {
    Serial.print("B4 X:");
    Serial.print(ircam.Blob4.X);
    Serial.print(" Y:");
    Serial.print(ircam.Blob4.Y);
    Serial.print(" S:");
    Serial.println(ircam.Blob4.Size);
  }

  delay(100);
}
