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

void setup(void) {

  Serial.begin(9600);

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

  Serial.begin(9600);
  Wire.begin();  // Initialize hardware I2C pins
  
  Wire.beginTransmission(DISPLAY);
  byte error = Wire.endTransmission();
  if (error != 0) {
    Serial.print(F("Couldn't find the display."));
    while(1);
  }

  clearDisplayI2C();  // Clears display, resets cursor
  setBrightnessI2C(255);  // High brightness
  delay(100);

  if(!bno.begin()) {
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }

  delay(1000);
  bno.setExtCrystalUse(true);

  uint8_t system, gyro, accel, mag = 0;
  while(mag != 3) {
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
}

void loop(void) {
  s7sSendStringI2C("-HI-");
  delay(1500);
  clearDisplayI2C();  
  delay(1500);

  InfraredResult InfraredBall1, InfraredBall2;
  
  if(SensorOne) {
    selectSeeker(0);
    InfraredBall1 = InfraredSeeker::ReadAC();
    reading1 = InfraredBall1.Direction - 5;
    strength1 = InfraredBall1.Strength / 8;
    if(reading1 == -5 || reading1 == 5) {
      reading1 = 0;
      strength1 = 0;
    }
  }

  if(SensorTwo) {
    selectSeeker(1);
    InfraredBall2 = InfraredSeeker::ReadAC();
    reading2 = InfraredBall2.Direction - 5;
    strength2 = InfraredBall2.Strength / 8;
    if(reading2 == -5 || reading2 == 5) {
      reading2 = 0;
      strength2 = 0;
    }
  }
  
  char buffer[100];
  if(SensorOne && !SensorTwo)
    sprintf(buffer,"%i,%i\n",reading1, strength1);
  if(!SensorOne && SensorTwo)
    sprintf(buffer,"%i,%i\n",reading2, strength2);
  if(SensorOne && SensorTwo)
    sprintf(buffer,"%i,%i,%i,%i\n",reading1, strength1, reading2, strength2);
    // Give status reading that we should move back
  Serial.print(buffer);

  uint16_t r, g, b, c, colorTemp, lux;
  
  tcs.getRawData(&r, &g, &b, &c);
  colorTemp = tcs.calculateColorTemperature(r, g, b);
  lux = tcs.calculateLux(r, g, b);
  
  Serial.print("Color Temp: "); Serial.print(colorTemp, DEC); Serial.print(" K - ");
  Serial.print("Lux: "); Serial.print(lux, DEC); Serial.print(" - ");
  Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
  Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
  Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");
  Serial.print("C: "); Serial.print(c, DEC); Serial.print(" ");
  Serial.println(" ");

  imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
  double heading = euler.x();
  Serial.print("Heading: ");
  Serial.println(heading);
  delay(BNO055_SAMPLERATE_DELAY_MS);

  drive(-50, 0, 0);
  delay(5000);
  halt();
  delay(2000);
  
}

 
void selectSeeker(uint8_t i) {
  if (i > 7) return;
  Wire.beginTransmission(MULTIPLEXER);
  Wire.write(1 << i);
  Wire.endTransmission();  
}


void s7sSendStringI2C(String toSend) {
  Wire.beginTransmission(DISPLAY);
  for (int i=0; i<4; i++)
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

