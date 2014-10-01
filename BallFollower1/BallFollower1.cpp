// Do not remove the include below
#include "BallFollower1.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
boolean seen = false;
int last_side = NONE;

void setup() {
  AFMS.begin();
  InfraredSeeker::Initialize();
}

void loop() {
  InfraredResult InfraredBall = InfraredSeeker::ReadAC();
  float reading = (InfraredBall.Direction - 5.0);
  float strength = (InfraredBall.Strength);
  if(strength == 0.0) {
     halt();
  } else {
    if(reading < 0) {
      last_side = LEFT;
      spin(-50);
    } else if(reading > 0) {
      last_side = RIGHT;
      spin(50);
    } else {
      if(strength < 200) {
        drive(0, 10, 0);
      }

    }
  }
}
