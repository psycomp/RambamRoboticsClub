int getInfrared() {
  Wire.requestFrom(0x50, 1);
  if (Wire.available()) {
    int index = Wire.read();
    Serial.print("Best Sensor: ");
    Serial.println(index);
    return index;
  }
}

