#include <Wire.h>                 // I2C Communication
#include <Adafruit_Sensor.h>      // Library for Adafruit Sensors
#include "Adafruit_TCS34725.h"    // RGB Sensor
#include <Adafruit_BNO055.h>      // Compass
#include <Adafruit_MotorShield.h> // Motors
#include <HTInfraredSeeker.h>     // Ball Sensor
#include <RunningAverage.h>       // to smooth out TSOP readings
#include <utility/imumaths.h>
#include <math.h>

#define DEBUG
#ifdef DEBUG
  #define DEBUG_PRINT(x)     Serial.print (x)
  #define DEBUG_PRINTDEC(x)  Serial.print (x, DEC)
  #define DEBUG_PRINTLN(x)   Serial.println (x)
#else
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTDEC(x)
  #define DEBUG_PRINTLN(x) 
#endif

// The off-switch is connected between pin 7 and ground.
#define OFF_SWITCH 7

#define DISPLAY 0x71 // 4-character display
#define MULTIPLEXER 0x70 // I2C multiplexer
#define IR_SENSOR 0x08 // Lego Sensors
#define IR_SENSOR_TSOP 0x50 // Homemade TSOP sensors

// Allows us to test if I2C devices are connected
extern "C" { 
  #include "utility/twi.h"
}

//////////////////////
// Global Variables //
//////////////////////
boolean SensorOne = false;
boolean SensorTwo = false;
boolean SensorTsop = false;
int front = 0;

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_BNO055 bno = Adafruit_BNO055();
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);

void setup(void) {
  setupEverything();
  front = getCompass();
}

void loop(void) {
  boolean offSwitch = getOffSwitch();
  int valueC = getCompass();
  int compOff = -(valueC - front);
  // display some things, make some choices, maybe change states

  if(offSwitch) {
    int sensor = getTSOP(3);
    int strength = getTSOPStrength(3);
    Serial.println(sensor);
    Serial.println(strength);
    if (sensor == 3 && strength > 9) {
      drive(0, 75, compOff);
      delay(200);
      drive(0, -75, compOff);
    }
    switch (sensor) {
    case 1:
      drive(-75, 0, compOff);
      break;
    case 2:
      drive(-75, 0, compOff);
      break;
    case 3:
      if (strength < 10) { halt(); }
      break;
    case 4:
      drive(75, 0, compOff);
      break;
    case 5:
      drive(75, 0, compOff);
      break;
    default:
      halt();
      break;
    }
  } else {
    // "PAUS" on display
  }
}

