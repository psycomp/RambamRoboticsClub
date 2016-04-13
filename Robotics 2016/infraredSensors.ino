int getInfrared() {
  int index = 3;
  Wire.requestFrom(0x50, 2);
  for (int i = 0; i <= 4; i++) { 
    if (Wire.available()) {
      index = Wire.read();
      ave.push(index);
    }
  }
  index = ave.mode();
  ave.clear();
  return (index == 0) ? 3 : index;
}

boolean getPixy() {
  uint16_t blocks;
  int total = 0;
  for(int i = 0; i <= 4; i++) {
    total += pixy.getBlocks();
    delay(10);
  }
  return(total > 0 ? true : false);
}

