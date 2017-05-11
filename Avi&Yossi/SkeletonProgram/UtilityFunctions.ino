// Send stuff to the display
void displayString(String toSend) {
  clearDisplayI2C();  
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

