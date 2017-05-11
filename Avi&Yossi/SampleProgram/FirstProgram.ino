#include <Wire.h>                 // I2C Communication
#include <Adafruit_Sensor.h>
#include "Adafruit_TCS34725.h"    // RGB Sensor
#include <Adafruit_BNO055.h>      // Compass
#include <Adafruit_MotorShield.h> // Motors
#include <HTInfraredSeeker.h>     // Ball Sensor
#include <utility/imumaths.h>
#include <math.h>

#define OFF_SWITCH 7
#define DISPLAY 0x71
#define MULTIPLEXER 0x70
#define BNO055_SAMPLERATE_DELAY_MS (100)

#define NO_STATE    0
#define STATE_ONE   1
#define STATE_TWO   2
#define STATE_THREE 3
#define STATE_FOUR  4
#define STATE_FIVE  5
#define STATE_SIX   6

// Allows us to test if I2C devices are connected
extern "C" { 
  #include "utility/twi.h"  // from Wire library, so we can do bus scanning
}

// Global Variables
Adafruit_BNO055 bno = Adafruit_BNO055();
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);

boolean SensorOne = false;
boolean SensorTwo = false;

void setup(void) {
  setupEverything();
  displayString("-OK-");
}

void loop(void) {
  int currentState = STATE_ONE, previousState = NO_STATE;
  boolean offSwitch = getOffSwitch();
  int valueA = getLegoSensor();
  int valueB = getColorSensor();
  int valueC = getCompass();

  // display some things, make some choices, do something
  
  drive(-50, 0, 0);
  delay(5000); // Delay is bad!!!!
  halt();
  delay(2000);  // Delay is bad!!!!

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
  }
}

