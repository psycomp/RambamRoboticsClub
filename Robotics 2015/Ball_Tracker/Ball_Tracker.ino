#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <HTInfraredSeeker.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
int _stopMode = RELEASE;

void setup() {
  Serial.begin(9600);
  // Wire.begin() is taken care of in AFMS.begin()
  AFMS.begin();
  InfraredSeeker::Initialize();
}

void loop() {
  InfraredResult InfraredBall = InfraredSeeker::ReadAC();
  float reading = (InfraredBall.Direction - 5.0);
  float strength = (InfraredBall.Strength);
  Serial.println(strength);
  
      if(reading < 0) {
          // It's off to the left.
        Serial.println("LEFT");
          spin(-30);
        } else if(reading > 0) {
          // It's off to the right.
        Serial.println("RIGHT");
          spin(30);
        } else {
          // We are looking at it.
        Serial.println("STOP");
          halt(); 
        } 
      
  
}
