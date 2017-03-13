#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

#define BNO055_SAMPLERATE_DELAY_MS (100)
Adafruit_BNO055 bno = Adafruit_BNO055();
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

void setup() {
  Serial.begin(9600);
  AFMS.begin();
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

void loop() {
  imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
  double heading = euler.x();
  Serial.print("Heading: "); Serial.println(heading);

  if(heading < 175) { 
    int r = map(heading, 0, 175, 50, 0);
    rDrive(180, 25, r); 
  } else if(heading > 185) {
    int r = map(heading, 185, 360, 0, -50);
    rDrive(180, 25, r); 
  } else {
    rDrive(180, 25, 0);
  }
  delay(BNO055_SAMPLERATE_DELAY_MS);
}
