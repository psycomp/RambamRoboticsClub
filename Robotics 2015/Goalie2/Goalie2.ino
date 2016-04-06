#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <HTInfraredSeeker.h>
#include <HMC58X3.h>
// #define DEBUG

HMC58X3 magn;
float initialCompass;

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
int irSensorPin = A0;

void setup() {
  #ifdef DEBUG
    Serial.begin(9600);
  #endif 
  AFMS.begin();
  InfraredSeeker::Initialize();
  magn.init(false); // Dont set mode yet, we'll do that later on.
  magn.calibrate(1); // Use gain 1=default, valid 0-7, 7 not recommended.
  magn.setMode(0);
  delay(10);
  initialCompass = getCompass();
}

void loop() {
  int _lineSensor = analogRead(irSensorPin);           // 0 to 1024
  if(_lineSensor < 970) { halt(); turnTo(0); return; }
  InfraredResult InfraredBall = InfraredSeeker::ReadAC();
  float _ballHeading = (InfraredBall.Direction - 5.0); // -4 to 4
  float _ballDistance = 255 - (InfraredBall.Strength); // 0 to 255
  float _facing = getCompass(initialCompass);          // -180.0 to 180.0

  #ifdef DEBUG
    Serial.print(_facing);
    Serial.print("\t");
    Serial.print(_ballHeading);
    Serial.print("\t");
    Serial.print(_ballDistance);
    Serial.print("\t");
    Serial.println(_lineSensor);
  #endif

  if(_ballDistance < 255) { // We can see the ball.

    if(abs(_facing) > 80.0) {
      #ifdef DEBUG
        Serial.println("CHARGE!"); 
      #endif
      
      drive(0, 50, 0);  // CHARGE
      delay(1500);
      halt();
      drive(0, -50, 0); // RETREAT
      delay(1350);
      halt();
      turnTo(0);        // FACE FORWARD
      return;           // RESTART LOOP
    }

    // Track it.
    if(_ballHeading < 0) {
      // It's off to the left.
      spin(-40);
    } else if(_ballHeading > 0) {
      // It's off to the right.
      spin(40);
    } else {
      // We are looking at it.
      halt(); 
    }   
  } else {
     // Can't see the ball. Don't move.
     halt();
  }
}

