#include <Wire.h>
#include <Adafruit_MotorShield.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
int _stopMode = BRAKE; // BRAKE or RELEASE must be set.

void setup() {
  Serial.begin(9600);
  AFMS.begin();
}

void loop() {
  rDrive(90, 25, 0); // 90-deg, right
  delay(1000);
  halt(); 
  delay(100);
  rDrive(180, 25, 0); // 180-deg, back
  delay(1000);
  halt(); 
  delay(100);
  rDrive(270, 25, 0); // 270-deg, left
  delay(1000);
  halt(); 
  delay(100);
  rDrive(360, 25, 0); // 360 or 0-deg, forward
  delay(1000);
  halt(); 
  delay(100);
}
