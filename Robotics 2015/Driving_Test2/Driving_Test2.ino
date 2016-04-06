#include <Wire.h>
#include <Adafruit_MotorShield.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
int _stopMode = BRAKE; // BRAKE or RELEASE must be set.

int a = 0;
int v = 100;
int r = 100;

void setup() {
  Serial.begin(9600);
  AFMS.begin();
}

void loop() {
  drive(-50, 0, 13);
  // rDrive(a, v, r);
}
