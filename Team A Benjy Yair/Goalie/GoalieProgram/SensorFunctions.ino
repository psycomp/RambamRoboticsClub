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

int getTSOP(int smoothing) {
  if(smoothing > 0) {
    RunningAverage index_ra(smoothing);
    RunningAverage strength_ra(smoothing);
    index_ra.clear();
    strength_ra.clear();
    for(int x = 0; x < smoothing; x++) {
      Wire.requestFrom(0x50, 2);
      if (Wire.available()) {
        int index = Wire.read();
        index_ra.addValue(index);
        int strength = Wire.read();
        strength_ra.addValue(strength);
      }
      // What do we return here??
      // convert to int from float
      return (int)index_ra.getAverage();
      // strength_ra.getAverage();
    }
  } else {
    Wire.requestFrom(0x50, 2);
    if (Wire.available()) {
      int index = Wire.read();
      int strength = Wire.read();
      return index;
    }
  }
}

int getTSOPStrength(int smoothing) {
  if(smoothing > 0) {
    RunningAverage strength_ra(smoothing);
    strength_ra.clear();
    for(int x = 0; x < smoothing; x++) {
      Wire.requestFrom(0x50, 2);
      if (Wire.available()) {
        int index = Wire.read();
        int strength = Wire.read();
        strength_ra.addValue(strength);
      }
      return (int)strength_ra.getAverage();
    }
  } else {
    Wire.requestFrom(0x50, 2);
    if (Wire.available()) {
      int index = Wire.read();
      int strength = Wire.read();
      return strength;
    }
  }
}
