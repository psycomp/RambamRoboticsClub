#include <Wire.h>
#include <Adafruit_MotorShield.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield();

void setup() {
  Serial.begin(9600);
  AFMS.begin();

  testAllWheels();
}

void loop() {
  drive(-50, 0, 0);
  delay(5000);
  halt();
  delay(2000);
}
