int getInfrared() {
  int index = 5;
  ave.clear();
  Wire.requestFrom(0x50, 2);
  for (int i = 0; i <= 14; i++) { 
    if (Wire.available()) {
      index = Wire.read();
      if(index == 5) { return 5; }     
      ave.push(index);
    }
  }
  index = ave.mode();
  return (index == 0) ? 5 : index;
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

