// Do you like it?  IT ACTUALLY WORKS!!

// state 0 is when were holding robot
// state 1 is when cant find ball
// state 2 is when not behind ball
// state 3 is when behind ball
// state 4 is on white line
#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <Adafruit_MotorShield.h>
#include <Wire.h>
#include <HTInfraredSeeker.h>
#include <Wire.h> // Include the Arduino SPI library

#define DISPLAY 0x71
#define BNO055_SAMPLERATE_DELAY_MS (100)
#define MULTIPLEXER 0x70

extern "C" {
#include "utility/twi.h"  // from Wire library, so we can do bus scanning
}

Adafruit_BNO055 bno = Adafruit_BNO055();
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);

boolean SensorOne = false;
boolean SensorTwo = false;

int reading1, strength1;
int reading2, strength2;
int state;
int reading;
int strength;
int a;
int v;
int r;

void setup(void) {

  while (!Serial);
  delay(500);
  Serial.begin(9600);
  Wire.begin();

  uint8_t data;
  selectSeeker(0);
  if (!twi_writeTo(MULTIPLEXER, &data, 0, 1, 1)) {
    SensorOne = true;
    InfraredSeeker::Initialize();
  }
  selectSeeker(1);
  if (!twi_writeTo(MULTIPLEXER, &data, 0, 1, 1)) {
    SensorTwo = true;
    InfraredSeeker::Initialize();
  }
  delay(500);

  Wire.beginTransmission(DISPLAY);
  byte error = Wire.endTransmission();
  if (error != 0) {
    Serial.print(F("Couldn't find the display."));
    while (1);
  }

  clearDisplayI2C();  // Clears display, resets cursor
  setBrightnessI2C(255);  // High brightness
  delay(100);

  if (!bno.begin()) {
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1);
  }

  delay(1000);
  bno.setExtCrystalUse(true);

  uint8_t system, gyro, accel, mag = 0;
  while (mag != 3) {
    bno.getCalibration(&system, &gyro, &accel, &mag);
    Serial.print(" Mag=");
    Serial.println(mag, DEC);
    delay(BNO055_SAMPLERATE_DELAY_MS);
  }

  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1);
  }

  // Now we're ready to get readings!

  AFMS.begin();
  s7sSendStringI2C("-HI-");
  delay(1500);
  clearDisplayI2C();
  delay(1500);
}

void loop(void) {

  // read sensors
  InfraredResult InfraredBall1, InfraredBall2;

  if (SensorOne) {
    selectSeeker(0);
    InfraredBall1 = InfraredSeeker::ReadAC();
    reading1 = InfraredBall1.Direction - 5;
    strength1 = InfraredBall1.Strength / 8;
    state = 2;
    if (reading1 == -5 || reading1 == 5) {
      reading1 = 0;
      strength1 = 0;
      state = 1;
    }
  } else if (SensorTwo) {
    selectSeeker(1);
    InfraredBall2 = InfraredSeeker::ReadAC();
    reading2 = InfraredBall2.Direction - 5;
    strength2 = InfraredBall2.Strength / 8;
    state = 2;
    if (reading2 == -5 || reading2 == 5) {
      reading2 = 0;
      strength2 = 0;
      state = 1;
    }
  } else  {
    state = 1;
  }

  char buffer[100];
  if (SensorOne && !SensorTwo)
    sprintf(buffer, "%i,%i\n", reading1, strength1);
  if (!SensorOne && SensorTwo)
    sprintf(buffer, "%i,%i\n", reading2, strength2);
  if (SensorOne && SensorTwo)
    sprintf(buffer, "%i,%i,%i,%i\n", reading1, strength1, reading2, strength2);
  // Give status reading that we should move back
  Serial.print(buffer);

  uint16_t r, g, b, c, colorTemp, lux;

  tcs.getRawData(&r, &g, &b, &c);

  Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
  Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
  Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");
  Serial.println(" ");
  if(r > 240 && b > 240 && c > 240) {
    state = 4;
  }

  imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
  double heading = euler.x();
  Serial.print("Heading: ");
  Serial.println(heading);
  //delay(BNO055_SAMPLERATE_DELAY_MS);



  // switch statement
  switch (state) {
    case 0:
      break;
    case 1:
      drive(0, -50, 0);
      break;
    case 2:
      reading1 = map(reading1, -5, 225, 5, 135);  // these are the values of the angles of the 5 and -5 zones (in a pie divided into 8 slices, each slice has 45 degrees)
      reading2 = map(reading2, 315, -1, 5, 45);   // we can change these values later if theyre wrong
      reading1 = strength1 >= 1 ? reading1 : reading2;
      reading2 = strength2 >= 1 ? reading2 : reading1;
      strength1 = strength1 >= 1 ? strength1 : strength2;
      strength2 = strength2 >= 1 ? strength2 : strength1;
      reading = reading1 + reading2 / 2;
      strength = strength1 + strength2 / 2;

      if(strength1 < 1 && strength2 < 1)  {
        state = 1;
        break;
      }

      if(strength > 5 && reading > 45 && reading < 135)  { // the strength number is arbitrary.  We have to figure out what the range of values for strength is.  The rest basically means we're not behind the ball.
        v = map(strength, 0, 0, 8, 50);  // The 8 is arbitrary too but it will be the highest possible strength.
        a = reading - 180 < 0 ? reading + 180 : reading - 180;  // Basically we go away from the ball at different speeds depending on how close behind the ball is.  We'll have to test this to see if it works
      } else if(reading < 45 && reading > 135) {
        v = map(strength, 0, 50, 8, 15); // if the ball is closer we go slower
        a = reading;  // go towards the ball
      } else  { // if we're behind the ball and we're not close to it
        a = 180;
        v = 50;  // go backwards so we can get behind it
      }
      rDrive(a, v, 0);
      break;
    case 4:
      a = reading - 180 < 0 ? reading + 180 : reading - 180;  // turn around
      rDrive(a, v, 0);
      break;
  }
}


void selectSeeker(uint8_t i) {
  if (i > 7) return;
  Wire.beginTransmission(MULTIPLEXER);
  Wire.write(1 << i);
  Wire.endTransmission();
}


void s7sSendStringI2C(String toSend) {
  Wire.beginTransmission(DISPLAY);
  for (int i = 0; i < 4; i++)
  {
    Wire.write(toSend[i]);
  }
  Wire.endTransmission();
}

void clearDisplayI2C() {
  Wire.beginTransmission(DISPLAY);
  Wire.write(0x76);  // Clear display command
  Wire.endTransmission();
}

void setBrightnessI2C(byte value) {
  Wire.beginTransmission(DISPLAY);
  Wire.write(0x7A);  // Set brightness command byte
  Wire.write(value);  // brightness data byte
  Wire.endTransmission();
}
