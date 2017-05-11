// Read the off switch
boolean getOffSwitch() {
  return digitalRead(OFF_SWITCH);
}

// Choose a lego sensor
void selectSeeker(uint8_t i) {
  if (i > 7) return;
  Wire.beginTransmission(MULTIPLEXER);
  Wire.write(1 << i);
  Wire.endTransmission();  
}

// Read the compass, figure out a good answer and return it
double getCompass() {
  imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
  double heading = euler.x();
  return heading;
}

// Read the color sensor, figure out a good answer and return it
int getColorSensor() {
  uint16_t r, g, b, c, colorTemp, lux;
  
  tcs.getRawData(&r, &g, &b, &c);
  colorTemp = tcs.calculateColorTemperature(r, g, b);
  lux = tcs.calculateLux(r, g, b);
  return 0;
}

// Read the sensors, figure out a value & send it back
// This is test code
int getLegoSensor() {
  int reading1, strength1;
  int reading2, strength2;  
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
  
  return 0;
}

