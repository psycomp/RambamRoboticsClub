
float getCompass() { return getCompass(-1.0); }

float getCompass(float initialReading) {
  float fx, fy, fz;
  magn.getValues(&fx,&fy,&fz);
  float _dir = atan2(fy, fx);
  if(_dir < 0) { _dir += 2 * M_PI; }
  _dir = ((_dir * 180/M_PI) - 180.0);
  if(initialReading >= 0) {
    _dir -= initialReading;
    if(_dir > 180) { _dir -= 360.0; }
    else if(_dir < -180) { _dir += 360.0; }
   }
   return _dir;
}

void turnTo(float _dir) {
  float myDir = getCompass(initialCompass);
  float offset = myDir - _dir;
  if(offset < -180.0) { offset += 360.0; }
  while(abs(offset) > 10) {
    Serial.println(offset);
    if(offset > 0) {
      spin(-20);
    } else if(offset < 0) {
      spin(20);
    }
    myDir = getCompass(initialCompass);
    offset = myDir - _dir;
    if(offset < -180.0) { offset += 360.0; }
  }
  halt();
}
