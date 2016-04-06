#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <HTInfraredSeeker.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
int _stop = 0;

void setup() {
  Serial.begin(9600); // DEBUG
  // Wire.begin() is taken care of in AFMS.begin()
  AFMS.begin();
  InfraredSeeker::Initialize();
}

void loop() {
  InfraredResult InfraredBall = InfraredSeeker::ReadAC();
  float reading = (InfraredBall.Direction - 5.0);
  float strength = (InfraredBall.Strength);

  if(_stop > 0 && millis() >= _stop) {
    halt();
    _stop = 0;
  }
  
  if(strength > 75) {
    // Ball is close enough to do something
    // Serial.println("CLOSE");
    
    if(strength > 120 && (reading >= -1 && reading <= 1)) {
      // Ball is *really* close and we are looking at it (or near to it)
    // Serial.println("CLOSER");
     
      if(reading == -1) { 
        // Ball is left of center
    Serial.println("LEFT");
        rDrive(345, 40, 0);
        _stop = millis() + 2000;
        // rDrive(165, 40, 0);
        // stopTime = millis() + 2000;
        // halt();      
      } else if(reading == 1) {
        // Ball is right of center
    Serial.println("RIGHT");
        rDrive(15, 40, 0);
        _stop = millis() + 2000;
        // rDrive(200, 40, 0);
        // stopTime = millis() + 2000;
        // halt();      
      } else {
        // Ball is straight ahead
    Serial.println("CENTER");
        rDrive(0, 40, 0);
        _stop = millis() + 2000;
        // rDrive(180, 40, 0);
        // stopTime = millis() + 2000;
        // halt();      
      }
    } else {
      // Ball is close but not that close. Track it.
      if(reading < 0) {
          // It's off to the left.
          spin(-30);
        } else if(reading > 0) {
          // It's off to the right.
          spin(30);
        } else {
          // We are looking at it.
          halt(); 
        } 
      } 
  }
}

