
float getCompass() { return getCompass(-1.0); }

float getCompass(float initialReading) {
  imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
  float _dir = euler.x();
  if(initialReading >= 0) {
    _dir -= initialReading;
    if(_dir > 180) { _dir -= 360.0; }
    else if(_dir < -180) { _dir += 360.0; }
   }
   return _dir;
}

void turnTo(float _dir) {
  float myDir = getCompass(initialDirection);
  float offset = myDir - _dir;
  if(offset < -180.0) { offset += 360.0; }
  while(abs(offset) > COMPASS_TOLERANCE) {
    if(offset > 0) {
      spin(-20);
    } else if(offset < 0) {
      spin(20);
    }
    myDir = getCompass(initialDirection);
    offset = myDir - _dir;
    if(offset < -180.0) { offset += 360.0; }
  }
  halt();
}
