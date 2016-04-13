#include <Wire.h>
#include <NewPing.h>
#include <SPI.h>
#include <Pixy.h>
#include <Adafruit_MotorShield.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <Average.h>

#define BNO055_SAMPLERATE_DELAY_MS (100)

#define DEBUG
#define SWITCH_PIN 31

#define RIGHT 1
#define LEFT  -1
#define CORRECT_RIGHT 0.25
#define CORRECT_LEFT  0.25
#define FORTH 1
#define BACK  -1
#define STOP  0

#define COMPASS_TOLERANCE 7
// Global Variables

Adafruit_BNO055 bno = Adafruit_BNO055();
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

NewPing sonarRight(3, 2);
NewPing sonarRear(5, 4);
NewPing sonarLeft(7, 6);

Pixy pixy;

Average<int> ave(5);

float initialDirection;
imu::Vector<3> euler;

int ballIndex;

int xVelocity, yVelocity, rVelocity;
unsigned long previousMillis = 0;        // will store last time LED was updated
const long interval = 1000;           // interval at which to blink (milliseconds)

void setup() {
  Serial.begin(9600);
  Wire.begin();

  if (!bno.begin()) {
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1);
  }
  bno.setExtCrystalUse(true);

  pixy.init();

  pinMode(31, INPUT_PULLUP);

  AFMS.begin();

  initialDirection = getCompass();
  delay(100);
}

void loop() {
  unsigned long currentMillis = millis();

  // right, left should be 55cm, rear should be ~21cm.
  int _distanceFromLeft = getLeftDistance();
  int _distanceFromRight = getRightDistance();
  int _distanceFromBack = getRearDistance();

  ballIndex = getInfrared();

  boolean seeBall = getPixy();

  int _switch = digitalRead(SWITCH_PIN);
  int goSwitch = (_switch == 0) ? 1 : 0;

  float facing = getCompass(initialDirection);

  delay(BNO055_SAMPLERATE_DELAY_MS);

  if (goSwitch == 0) {
    halt();
    delay(10);
    return;
  }

  if (seeBall) {
    drive(0, 75, 0);
    delay(750);
    drive(0, -75, 0);
    delay(750);
    halt();
    return;
  }

  int rightLeft = STOP;
  int backForth = STOP;

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    float _RLheading = getCompass(initialDirection);
    if (abs(_RLheading) >= COMPASS_TOLERANCE) {
      getDriveValues(&xVelocity, &yVelocity, &rVelocity);
      turnTo(0.00);
      drive(xVelocity, yVelocity, rVelocity);
    }
  }

  int backToleranceMax = (_distanceFromLeft < 59 || _distanceFromRight < 59) ? 20 : 36;
  int backToleranceMin = (_distanceFromLeft < 59 || _distanceFromRight < 59) ? 15 : 31;
  
  if (_distanceFromBack > backToleranceMax) {
    backForth = BACK;
  }
  else if (_distanceFromBack < backToleranceMin) {
    backForth = FORTH;
  }

  float _RLheading = getCompass(initialDirection);
  Serial.println(_RLheading);

  if (abs(_RLheading) < COMPASS_TOLERANCE) {
    if (ballIndex < 3) {
      rightLeft = LEFT;
    } else if (ballIndex > 3) {
      rightLeft = RIGHT;
    }

    if (rightLeft == LEFT && _distanceFromLeft < 20) {
      rightLeft = CORRECT_RIGHT;
    }
    else if (rightLeft == RIGHT && _distanceFromRight < 20) {
      rightLeft = CORRECT_LEFT;
    }
  }

  drive(75 * rightLeft, 30 * backForth, 0);

}

