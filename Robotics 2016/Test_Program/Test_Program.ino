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

// Global Variables

Adafruit_BNO055 bno = Adafruit_BNO055();
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

NewPing sonarRight(3,2);
NewPing sonarRear(5,4);
NewPing sonarLeft(7,6);

Pixy pixy;

Average<int> ave(5);

float initialDirection;
imu::Vector<3> euler;

int ballIndex;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  if(!bno.begin()) {
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  bno.setExtCrystalUse(true);

  pixy.init();

  pinMode(31, INPUT_PULLUP);

  AFMS.begin();

  initialDirection = getCompass();
  delay(100);
}

void loop() {
  // Read all Sensors //

  // - Sonar
  int left, right, rear;
  left = getLeftDistance();
  right = getRightDistance();
  rear = getRearDistance();

  // right, left & rear should be ~21cm.
  
//  Serial.print("RIGHT: ");
//  Serial.print(right);
//  Serial.print("  LEFT: ");
//  Serial.print(left);
//  Serial.print("  REAR: ");
//  Serial.println(rear);

  // - Ball Sensor
  ballIndex = getInfrared();
//  Serial.print("BALL: ");
//  Serial.println(ballIndex);

  // - Pixy
  boolean seeBall = getPixy();
//  Serial.println(seeBall);

  // - Switch
  int goSwitch = digitalRead(31);
  Serial.println(goSwitch);

  // - Compass
  float facing = getCompass(initialDirection);

  delay(BNO055_SAMPLERATE_DELAY_MS);
}

