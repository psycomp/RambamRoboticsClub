#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <HTInfraredSeeker.h>
#include <HMC58X3.h>
#include <Timer.h>

#define DEBUG

#ifdef DEBUG
  #define DEBUG_PRINT(x)  Serial.println (x)
#else
  #define DEBUG_PRINT(x)
#endif

#define TRACKING   1
#define CHARGING   2
#define RETREATING 3

#define IR_LEVEL 970

HMC58X3 magn;
float initialCompass;

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
int irSensorPin = A0;

Timer t;
int goalieEvent;
int goalieState = TRACKING;

void setup() {
  #ifdef DEBUG
    Serial.begin(9600);
  #endif 
  AFMS.begin(); // Initializes Wire for us.
  InfraredSeeker::Initialize();
  magn.init(false); // Dont set mode yet, we'll do that later on.
  magn.calibrate(1); // Use gain 1=default, valid 0-7, 7 not recommended.
  magn.setMode(0);
  delay(10);
  initialCompass = getCompass();
  
  DEBUG_PRINT("Running...");
}

void loop() {
  // Reading line sensor is the highest priority
  int _lineSensor = analogRead(irSensorPin); // 0 to 1024

  if(_lineSensor < IR_LEVEL) {
    halt();
    drive(0, -40, 0);
    delay(1000);
    goalieStop();
    return;
  }
  
  if(goalieState > TRACKING) {
    // We are not on the line
    // We are in the middle of the goalie-dance
    t.update();
    return;
  }
  
  // We're not doing the goalie-dance
  // We're not on the white line.
  // Behave normally.
  
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

    if(abs(_facing) > 75.0) {
      // We are turned more than 75-degrees from forward
      // Start the goalie-dance
      goalieCharge();
      return;
    }

    // Track the ball
    
    if(_ballHeading < 0) {
      DEBUG_PRINT("LEFT!"); 
      spin(-40);
    } else if(_ballHeading > 0) {
      DEBUG_PRINT("RIGHT!"); 
      spin(40);
    } else {
      DEBUG_PRINT("LOCKED ON!"); 
      halt(); 
    }   
  } else {
    // Can't see the ball. Don't move.
    DEBUG_PRINT("WAITING..."); 
    halt();
  }
}

