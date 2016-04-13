#include <Wire.h>
#include <SPI.h>
#include <Pixy.h>
#include <Adafruit_MotorShield.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <Average.h>

#define BNO055_SAMPLERATE_DELAY_MS (100)

#define DEBUG
#define SWITCH_PIN 7


#define COMPASS_TOLERANCE 7
// Global Variables

Adafruit_BNO055 bno = Adafruit_BNO055();
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

Pixy pixy;

Average<int> ave(15);

float initialDirection;
imu::Vector<3> euler;

int ballIndex;

int lookup[10][2] = {
  {0, 0}, 
  {50, -50}, //1
  {0, -50},  //2
  {-50, -50},//3
  {-50, 50},  //4
  {0, 50},   //5
  {50, 50},   //6
  {50, -50}, //7
  {0, -50},  //8
  {-50, -50} //9
};

void setup() {
  Serial.begin(9600);
  Wire.begin();

  if (!bno.begin()) {
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1);
  }
  bno.setExtCrystalUse(true);

  pixy.init();

  pinMode(SWITCH_PIN, INPUT_PULLUP);

  AFMS.begin();

  initialDirection = getCompass();
  delay(100);
}

void loop() {
  int ballIndex = getInfrared();
  float facing = getCompass(initialDirection);
  boolean seeBall = getPixy();
  
  Serial.println(seeBall); return;

  int _switch = digitalRead(SWITCH_PIN);
  int goSwitch = (_switch == 0) ? 1 : 0;

  if (goSwitch == 0) {
    halt();
    initialDirection = getCompass();
    delay(100);
    return;
  }

  float _RLheading = getCompass(initialDirection);
  if (abs(_RLheading) >= COMPASS_TOLERANCE) {
    turnTo(0.00);
  }

  _RLheading = getCompass(initialDirection);

  int x = lookup[ballIndex][0];
  int y = lookup[ballIndex][1];
  Serial.print(ballIndex);
  Serial.print(":  X: ");
  Serial.print(x);
  Serial.print("  Y: ");
  Serial.println(y);
  drive(x, y, 0);
  delay(100);
}

