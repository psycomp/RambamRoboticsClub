// case1 0 is not found ball
// case1 1 is found ball
// case1 2 is on white line
#define DRIVE_SPEED 100

#include <Wire.h>
#include <HTInfraredSeeker.h>
#include "Adafruit_TCS34725.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

void setup  {
  setupSensors()
}

void loop {
  
}

int setupSensors  {
  extern "C" { 
#include "utility/twi.h"  // from Wire library, so we can do bus scanning
}

#define MULTIPLEXER 0x70

boolean SensorOne = false;
boolean SensorTwo = false;
 
void selectSeeker(uint8_t i) {
  if (i > 7) return;
  Wire.beginTransmission(MULTIPLEXER);
  Wire.write(1 << i);
  Wire.endTransmission();  
}

void setup() {
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
}
  int reading1, strength1;
  int reading2, strength2;
Adafruit_TCS34725 tcs = Adafruit_TCS34725();
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);

  Serial.begin(9600);
  
  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1);
  }
  
  // Now we're ready to get readings!
}
#define BNO055_SAMPLERATE_DELAY_MS (100)
Adafruit_BNO055 bno = Adafruit_BNO055();

  Serial.begin(9600);

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
}

void loop {
  readSensors();
  switch(case1) {
    case 0:
    break;
    case 1:
      drive(0, DRIVE_SPEED, 0);
      break;
    case 2;
      //write code for if behind ball and facing right direction
      if(!(Heading >= 356 || Heading <= 4)){
        int ROTATE_SPEED = map(Heading, 0, 360, 100, -100);
        spin(ROTATE_SPEED);
         
      }
  }
}




int readSensors() {
  // set up switch here

  // Lego sensor
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
  if(SensorOne && !SensorTwo){
    sprintf(buffer,"%i,%i\n",reading1, strength1);
    case1 = 2;
  }
  if(!SensorOne && SensorTwo){
    sprintf(buffer,"%i,%i\n",reading2, strength2);
    case1 = 2;
  }
  if((SensorOne && SensorTwo) || (!SensorOne && !SensorTwo))  {
    //sprintf(buffer,"%i,%i,%i,%i\n",reading1, strength1, reading2, strength2);
    case1 = 1 // not found ball
  }
  Serial.print(buffer);

  // Color Sensor
  uint16_t r, g, b;
  tcs.getRawData(&r, &g, &b);
  
  Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
  Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
  Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");
  Serial.println(" ");

  if(r > 235 && g > 235 && b > 235) {
    case1 = 4 // on white line
  }
  // Compass
  imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
  double heading = euler.x();
  Serial.print("Heading: ");
  Serial.println(heading);  // 0-360
  delay(BNO055_SAMPLERATE_DELAY_MS);
}
