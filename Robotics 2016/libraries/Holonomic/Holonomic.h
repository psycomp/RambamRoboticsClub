/*
  Holonomic.h - Library for Holonomic Drive
  Motor agnostic. Assumes x/y/velocity/rotation inputs are -100 to 100
                  Assumes angular inputs are 0-360
  Created by Steven Weinberger, September 19, 2014.
  Released into the public domain.
*/
#ifndef Holonomic_h
#define Holonomic_h

#include "Arduino.h"
#include <Adafruit_MotorShield.h>
#include <math.h>

class Holonomic {
  public:
    Holonomic(Adafruit_MotorShield AFMS);
    void drive(int x, int y, int r);
    void rDrive(int a, int v, int r);
    void halt();
    void spin(int r);
  private:
    Adafruit_MotorShield _afms;
    Adafruit_DCMotor *motor1;
    Adafruit_DCMotor *motor2;
    Adafruit_DCMotor *motor3;
    Adafruit_DCMotor *motor4;
};

#endif
