/*
Send stuff to the display
Can display any digit or AbCcdEeFGHhIiJLlnOoPqrStUuY, e.g.
  bALL
  GOAL
  FInd
  OFF
  On
  GO/Go
  StoP
  LInE
  Err
  -1-
  -2-
  -3-
  -4-
  -5-
  -6-
  -7-
  -8-
 */
void displayString(String toSend) {
  clearDisplay();  
  Wire.beginTransmission(DISPLAY);
  for (int i=0; i<4; i++)
  {
    Wire.write(toSend[i]);
  }
  Wire.endTransmission();
}

void clearDisplay() {
  Wire.beginTransmission(DISPLAY);
  Wire.write(0x76);  // Clear display command
  Wire.endTransmission();
}

void setDisplayBrightness(byte value) {
  Wire.beginTransmission(DISPLAY);
  Wire.write(0x7A);  // Set brightness command byte
  Wire.write(value);  // brightness data byte
  Wire.endTransmission();
}

