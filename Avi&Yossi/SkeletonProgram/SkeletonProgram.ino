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

// Our states
#define NO_STATE    0
#define STATE_ONE   1
#define STATE_TWO   2
#define STATE_THREE 3
#define STATE_FOUR  4
#define STATE_FIVE  5
#define STATE_SIX   6

#define DISPLAY 0x71 // 4-character display
#define MULTIPLEXER 0x70 // I2C multiplexer
#define IR_SENSOR 0x08 // Lego Sensors
#define IR_SENSOR_TSOP 0x50 // Homemade TSOP sensors

// Allows us to test if I2C devices are connected
extern "C" { 
  #include "utility/twi.h"
}

// Global Variables
int currentState = STATE_ONE, previousState = NO_STATE;

//////////////////////
// Global Variables //
//////////////////////
boolean SensorOne = false;
boolean SensorTwo = false;
boolean SensorTsop = false;

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_BNO055 bno = Adafruit_BNO055();
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);

void setup(void) {
  setupEverything();
}

void loop(void) {
  boolean offSwitch = getOffSwitch();
  int valueA = getLegoSensor();
  int valueB = getColorSensor();
  int valueC = getCompass();

  // display some things, make some choices, maybe change states
  
  drive(-50, 0, 0);
  delay(5000); // Delay is bad!!!!
  halt();
  delay(2000);  // Delay is bad!!!!

  if(offSwitch == true) {
    // do something;
  } else {
    // do something else;
  }
  
  switch(currentState) {
    case STATE_ONE:
      // stuff
      break;
    case STATE_TWO:
      // stuff
      break;
    case STATE_THREE:
      // stuff
      break;
    case STATE_FOUR:
      // stuff
      break;
    case STATE_FIVE:
      // stuff
      break;
    case STATE_SIX:
      // stuff
      break;
    default:
      // stuff
      break;
  }
}

