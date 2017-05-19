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
  int compOff = (valueC - front);// / 20;
  if (compOff < -180) {
    compOff += 360;
  } else if (compOff > 180) {
    compOff -= 360;
  }
  return compOff;
}

int  vecLookup[10][2] = {
  //    {x, y}
  /*0*/ {0, 0},
  /*1*/ {-MOTOR_SPEED / 5, -MOTOR_SPEED},
  /*2*/ {0, -MOTOR_SPEED},
  /*3*/ {-MOTOR_SPEED, -MOTOR_SPEED},
  /*4*/ {-MOTOR_SPEED, MOTOR_SPEED / 2},
  /*5*/ {0, MOTOR_SPEED},
  /*6*/ {MOTOR_SPEED, MOTOR_SPEED / 2},
  /*7*/ {MOTOR_SPEED, -MOTOR_SPEED},
  /*8*/ {MOTOR_SPEED / 5, -MOTOR_SPEED},
  /*9*/ {0, -MOTOR_SPEED},
};

#define RIGHT 0
#define LEFT 1

int lastXDir = LEFT;

#define FWD 0
#define BKWD 1

int lastYDir = FWD;

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
      displayString("FOrD");
      paused = false;
    }
    int sensor = getTSOP(5);
    int strength = getTSOPStrength(5);
    Serial.println(sensor);
    Serial.println(strength);
    uint16_t r, g, b, c;
    tcs.getRawData(&r, &g, &b, &c);
//    clearDisplay();
//    displayString(String(String(c).length()) + String(c));
    if (abs(compOff) > 15 || (sensor == 0 && rot != 0)) {
      drive(0, 0, rot);
    } else {
      int x = vecLookup[sensor][0];
      int y = vecLookup[sensor][1];
//      if (x > 0) {
//        if (c > WHITE) {
//          if (lastDir == RIGHT) {
//           x = 0;
//          }
//        }
//        lastDir = RIGHT;
//      } else if (x < 0) {
//        if (c > WHITE) {
//          if (lastDir == LEFT) {
//            x = 0;
//          }
//        }
//        lastDir = LEFT;
//      }
      if (c > WHITE) {
        if ((x > 0 && lastXDir == RIGHT) || (x < 0 && lastXDir == LEFT)) {
          x = 0;
        }
        if ((y > 0 && lastYDir == FWD) || (y < 0 && lastYDir == BKWD)) {
          y = 0;
        }
      }
      if (x != 0) { lastXDir = x > 0 ? RIGHT : LEFT; }
      if (y != 0) { lastYDir = y > 0 ? FWD : BKWD; }
      drive(x, y, 0);
    }
  } else {
    if (!paused) {
      halt();
      displayString("pAUS");
      paused = true;
    }
  }
}

