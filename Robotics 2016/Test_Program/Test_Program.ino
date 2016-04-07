#include <Wire.h>
#include <NewPing.h>
#include <SPI.h>  
#include <Pixy.h>
#include <Adafruit_MotorShield.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

#define BNO055_SAMPLERATE_DELAY_MS (100)

#define DEBUG

// Global Variables

Adafruit_BNO055 bno = Adafruit_BNO055();
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

NewPing sonarRight(30, 31);
NewPing sonarLeft(32, 33);
NewPing sonarRear(34, 35);

Pixy pixy;

float initialCompass;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  if(!bno.begin()) {
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  bno.setExtCrystalUse(true);

  pixy.init();

  delay(100);
}

void loop() {
  // Read all Sensors
  // - Ball Sensor
  // - Sonar
  // - Compass
  // - Pixy

  // Do Something
}

