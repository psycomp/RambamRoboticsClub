#include <Wire.h>                 // I2C Communication
#include <Adafruit_Sensor.h>      // Library for Adafruit Sensors
#include "Adafruit_TCS34725.h"    // RGB Sensor
#include <Adafruit_BNO055.h>      // Compass
#include <Adafruit_MotorShield.h> // Motors
#include <HTInfraredSeeker.h>     // Ball Sensor
//#include <RunningAverage.h>       // to smooth out TSOP readings
#include <utility/imumaths.h>
#include <math.h>

#define WHITE_COLOR_LEVEL 2000 // THis is the amount of r g and b that the sensor picks up when on the white line
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
#define NO_STATE    0  // Holding Ball
#define STATE_ONE   1  // Ball Not Found
#define STATE_TWO   2  // Ball Found
#define STATE_THREE 3  // On White Line
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
int movingDirectionX = 0;
int movingDirectionY = 0;
int previousDirectionX = 0;
int previousDirectionY = 0;
//boolean SensorTsop = false;

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_BNO055 bno = Adafruit_BNO055();
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);

void setup(void) {
  setupEverything();
}

void loop(void) {
  boolean offSwitch = getOffSwitch();
  int reading = getLegoSensor();
  int heading = getCompass();

  // display some things, make some choices, maybe change states
  Serial.print("Reading: ");
  Serial.println(reading);
  
  if(getColorSensor())  {
    currentState = 3;
  }
  
  if(offSwitch == true) {
    // do something;
    drive(0,0,0); // Stop
  } else {
    // do something else;
  }
  
  switch(currentState) {
    case STATE_ONE:
      drive(0, -50, 0);
      break;
    case STATE_TWO:
      if(abs(reading) > 3) { // Drive to the side because the ball is behind us and we need to make sure not to hit it into our own goal.
        drive(reading / abs(reading) * 75, 0, 0);  // IF the reading is negative, reading / abs(reading) will return -1.  If not, itll return +1.
        movingDirectionX = reading / abs(reading) * 75;
        movingDirectionY = 0;
        break;
      } else if(abs(reading >= 1)) {
        drive(0, 75, 0);  // Drive forward because we're right behind the ball.
        movingDirectionX = 0;
        movingDirectionY = 75;
        break;
      } else {
        drive(-(reading / abs(reading)) * 50, -75, 0); // Drive backward while slowly moving sideways towards the ball because we're somewhere next to the ball.
        movingDirectionX = -(reading / abs(reading)) * 50;
        movingDirectionY = -75;
        break;
      }
      break;
    case STATE_THREE:
      // stuff
      drive(-movingDirectionX, -movingDirectionY, 0);
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
  previousDirectionX = movingDirectionX;
  previousDirectionY = movingDirectionY;
}
