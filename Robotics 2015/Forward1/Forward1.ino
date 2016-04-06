#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <HTInfraredSeeker.h>
#include <HMC58X3.h>

#define DEBUG

#ifdef DEBUG
  #define DEBUG_PRINT(x)  Serial.println (x)
#else
  #define DEBUG_PRINT(x)
#endif

#define IR_LEVEL 970

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
  float _facing = getCompass(initialCompass);          // -180.0 to 180.0

  if(_lineSensor < IR_LEVEL) {
    halt();
    turnTo(_facing + 180.0);
    drive(0, 40, 0);
    delay(1500);
    halt();
    turnTo(_facing);
    return;
  }

  InfraredResult InfraredBall = InfraredSeeker::ReadAC();
  float _ballHeading = (InfraredBall.Direction - 5.0); // -4 to 4
  float _ballDistance = 255 - (InfraredBall.Strength); // 0 to 255

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

    // Track it.
    if(_ballHeading < 0 && _ballDistance >= 120) {
      DEBUG_PRINT("LEFT!"); 
      spin(-30);
    } 
    else if(_ballHeading > 0 && _ballDistance >= 120) {
      DEBUG_PRINT("RIGHT!"); 
      spin(30);
    }
    else {
      // The ball is straight ahead
      
      if(abs(_facing) <= 90.0) {
        // We are facing their side 
        DEBUG_PRINT("CHARGE!"); 
        drive(0, 50, 0);
        delay(200);
        if(abs(_facing) < 10) {
          turnTo(0);
        } 
      }
      else if(abs(_facing) > 150) {
        // We are facing our side
        halt();
        delay(500);
        return;
      }
      
/*      
      else {
        DEBUG_PRINT("LOCKED ON!"); 
        drive(0, 35, 0);
        delay(1000);
        halt();
        delay(500);
        return;
      } 
*/
    }   
  }
  /*else {
     DEBUG_PRINT("WAITING..."); 
     halt();
  }*/
}

