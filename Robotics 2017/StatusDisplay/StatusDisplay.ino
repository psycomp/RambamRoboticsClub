#include <Wire.h> // Include the Arduino SPI library
extern "C" { 
#include "utility/twi.h"  // from Wire library, so we can do bus scanning
}

#define DISPLAY 0x71

void setup() {
  Serial.begin(9600);
  Wire.begin();  // Initialize hardware I2C pins
  
  Wire.beginTransmission(DISPLAY);
  byte error = Wire.endTransmission();
  if (error != 0) {
    Serial.print(F("Couldn't find the display."));
    while(1);
  }

  clearDisplayI2C();  // Clears display, resets cursor
  setBrightnessI2C(255);  // High brightness
  delay(100);
}

void loop() {
  s7sSendStringI2C("-HI-");
  delay(1500);
  clearDisplayI2C();  
  delay(1500);
}

void s7sSendStringI2C(String toSend) {
  Wire.beginTransmission(DISPLAY);
  for (int i=0; i<4; i++)
  {
    Wire.write(toSend[i]);
  }
  Wire.endTransmission();
}

void clearDisplayI2C() {
  Wire.beginTransmission(DISPLAY);
  Wire.write(0x76);  // Clear display command
  Wire.endTransmission();
}

void setBrightnessI2C(byte value) {
  Wire.beginTransmission(DISPLAY);
  Wire.write(0x7A);  // Set brightness command byte
  Wire.write(value);  // brightness data byte
  Wire.endTransmission();
}

