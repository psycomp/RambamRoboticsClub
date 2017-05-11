boolean setupEverything() {
  while (!Serial);
  delay(500);
  Serial.begin(9600);

  Wire.begin();
  AFMS.begin();

  // Do we have the display?
  Wire.beginTransmission(DISPLAY);
  byte error = Wire.endTransmission();
  if (error != 0) {
    Serial.print(F("Couldn't find the display."));
    while(1);
  }
  clearDisplayI2C();  // Clears display, resets cursor
  setBrightnessI2C(255);  // High brightness
  delay(100);

  // Do we have IR Sensor #1?
  uint8_t data;
  selectSeeker(0);
  if (!twi_writeTo(MULTIPLEXER, &data, 0, 1, 1)) {
    SensorOne = true;
    InfraredSeeker::Initialize();
  }
  delay(100);

  // Do we have IR Sensor #2?
  selectSeeker(1);
  if (!twi_writeTo(MULTIPLEXER, &data, 0, 1, 1)) {
    SensorTwo = true;
    InfraredSeeker::Initialize();
  }
  delay(100);

  if(!(SensorOne || SensorTwo)) {
    Serial.print(F("Couldn't find any IR sensors."));
    while(1);
  }

  // Do we have a compass?
  if(!bno.begin()) {
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  delay(500);
  bno.setExtCrystalUse(true);

  uint8_t system, gyro, accel, mag = 0;
  while(mag != 3) {
    bno.getCalibration(&system, &gyro, &accel, &mag);
    Serial.print(" Mag=");
    Serial.println(mag, DEC);    
    delay(BNO055_SAMPLERATE_DELAY_MS);
  }
  delay(100);

  // Do we have a color sensor?
  if (!tcs.begin()) {
    Serial.println("No TCS34725 found ... check your connections");
    while (1);
  }
  
}

