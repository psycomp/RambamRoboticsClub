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

#define MOTOR_SPEED 100
#define BLACK 12 // < 12
#define WHITE 29 // > 29

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
Adafruit_TCS34725 tcs = Adafruit_TCS34725();

void setup(void) {
  setupEverything();
  displayString("rEdY");
  while (getOffSwitch() == false) {}
//  displayString("Cp 5");
//  delay(1000);
//  displayString("Cp 4");
//  delay(1000);
  displayString("Cp 3");
  delay(1000);
  displayString("Cp 2");
  delay(1000);
  displayString("Cp 1");
  delay(1000);
  front = getCompass();
}

boolean paused = true;

int getCompOff() {
  int valueC = getCompass();
  int compOff = (valueC - front);
  if (compOff < -180) {
    compOff += 360;
  } else if (compOff > 180) {
    compOff -= 360;
  }
  return compOff;
}

int getColor() {
    uint16_t r, g, b, c;
    tcs.getRawData(&r, &g, &b, &c);
    return c;
}

#define RIGHT 0
#define LEFT 1

int lastXDir = LEFT;

void loop(void) {
  boolean offSwitch = getOffSwitch();
  int compOff = getCompOff();
//  clearDisplay();
//  displayString(String(String(compOff).length()) + String(compOff));
  int rot = 0;
  if (compOff > 15) {
    rot = -50;
  } else if (compOff < -15) {
    rot = 50;
  } else if (compOff > 10) {
    rot = -25;
  } else if (compOff < -10) {
    rot = 25;
  }

  if(offSwitch) {
    if (paused) {
      displayString("GOAL");
      paused = false;
    }
    int sensor = getTSOP(5);
    int strength = getTSOPStrength(5);
    Serial.println(sensor);
    Serial.println(strength);
    int c = getColor();
    if (sensor == 3 && strength > 10) {
      drive(0, MOTOR_SPEED, 0);
      delay(350);
      drive(0, -MOTOR_SPEED, 0);
      while (getColor() > BLACK) {}
//      delay(350);
    }
    if (abs(compOff) > 15) {
      drive(0, 0, rot);
      //while (abs(getCompOff()) > 10) {}
    } else {
      switch (sensor) {
      case 1:
        if (strength > 5 && !(c > WHITE && lastXDir == LEFT)) {
          drive(-MOTOR_SPEED, 0, 0);
          lastXDir = LEFT;
        } else if (rot != 0) {
          drive(0, 0, rot);
        }
        break;
      case 2:
        if (strength > 14 && !(c > WHITE && lastXDir == LEFT)) {
          drive(-MOTOR_SPEED, 0, 0);
          lastXDir = LEFT;
        } else if (strength > 5 && !(c > WHITE && lastXDir == LEFT)) {
          drive(-MOTOR_SPEED, 0, 0);
          lastXDir = LEFT;
        } else if (rot != 0) {
          drive(0, 0, rot);
        }
        break;
      case 3:
        if (strength < 11) {
          drive(0, 0, rot);
        }
        break;
      case 4:
        if (strength > 14 && !(c > WHITE && lastXDir == RIGHT)) {
          drive(MOTOR_SPEED, 0, 0);
          lastXDir = RIGHT;
        } else if (strength > 5 && !(c > WHITE && lastXDir == RIGHT)) {
          drive(MOTOR_SPEED, 0, 0);
          lastXDir = RIGHT;
        } else if (rot != 0) {
          drive(0, 0, rot);
        }
        break;
      case 5:
        if (strength > 5 && !(c > WHITE && lastXDir == RIGHT)) {
          drive(MOTOR_SPEED, 0, 0);
          lastXDir = RIGHT;
        } else if (rot != 0) {
          drive(0, 0, rot);
        }
        break;
      default:
        drive(0, 0, rot);
        break;
      }
    }
  } else {
    if (!paused) {
      halt();
      displayString("pAUS");
      paused = true;
    }
  }
}

