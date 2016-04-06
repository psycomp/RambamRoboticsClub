#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <HTInfraredSeeker.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
int _stopMode = RELEASE;
int _stopTime;
int sensorPin = A0;

void setup() {
  Serial.begin(9600);
  AFMS.begin();
  InfraredSeeker::Initialize();
}

void loop() {
  int lineSensor = 0;
  InfraredResult InfraredBall = InfraredSeeker::ReadAC();
  float heading = (InfraredBall.Direction - 5.0);
  float distance = 255 - (InfraredBall.Strength);
  lineSensor = analogRead(sensorPin);    

  Serial.print(heading);
  Serial.print("\t");
  Serial.print(distance);
  Serial.print("\t");
  Serial.println(lineSensor);

  if(distance < 255) {
    if(heading < 0) {
      // It's off to the left.
      Serial.println("LEFT");
      spin(-30);
    } else if(heading > 0) {
      // It's off to the right.
      Serial.println("RIGHT");
      spin(30);
    } else {
      // We are looking at it.
      Serial.println("STOP");
      halt(); 
    }   
  }
  
}
