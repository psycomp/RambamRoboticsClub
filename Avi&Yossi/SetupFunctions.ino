boolean setupEverything() {
  while (!Serial);
  delay(500);
  Serial.begin(9600);
  DEBUG_PRINTLN("Starting up...");

  // The cut-off switch is connected between pin 7 & ground
  // It defaults to HIGH, because of the built-in pullup resistor
  pinMode(OFF_SWITCH, INPUT_PULLUP);

  // Start I2C system
  Wire.begin();
  DEBUG_PRINTLN("Wire up...");

  // Initialize the motor shield
  AFMS.begin();
  DEBUG_PRINTLN("Motor Shield up...");

  // Do we have the display?
  Wire.beginTransmission(DISPLAY);
  byte error = Wire.endTransmission();
  if (error != 0) {
    DEBUG_PRINTLN("Couldn't find the display.");
    while(1);
  } else {
    DEBUG_PRINTLN("Found the display.");
  }
  clearDisplay();  // Clears display, resets cursor
  setDisplayBrightness(255);  // High brightness
  delay(100);

  // Do we have IR Sensor #1?
  uint8_t data;
  selectSeeker(0);
  if (!twi_writeTo(IR_SENSOR, &data, 0, 1, 1)) {
    SensorOne = true;
    InfraredSeeker::Initialize();
    DEBUG_PRINTLN("Found IR sensor #1.");
  } else {
    DEBUG_PRINTLN("Couldn't find IR sensor #1.");
  }
  delay(100);

  // Do we have the I2C Multiplexer?
  if (!twi_writeTo(MULTIPLEXER, &data, 0, 1, 1)) {
    DEBUG_PRINTLN("Found I2C Multiplexer.");
  } else {
    DEBUG_PRINTLN("Couldn't find I2C Multiplexer.");
  }
  delay(100);
  
  // Do we have IR Sensor #2?
  selectSeeker(1);
  if (!twi_writeTo(IR_SENSOR, &data, 0, 1, 1)) {
    SensorTwo = true;
    InfraredSeeker::Initialize();
    DEBUG_PRINTLN("Found IR sensor #2.");
  } else {
    DEBUG_PRINTLN("Couldn't find IR sensor #2.");
  }
  delay(100);

/*  // Do we have TSOP Sensors?
  if (!twi_writeTo(IR_SENSOR_TSOP, &data, 0, 1, 1)) {
    SensorTsop = true;
    DEBUG_PRINTLN("Found TSOP sensor.");
  } else {
    DEBUG_PRINTLN("Couldn't find TSOP sensor.");
  }
  delay(100);

  if(!(SensorOne || SensorTwo || SensorTsop)) {
    DEBUG_PRINTLN("Couldn't find any IR sensors.");
    while(1);
  }
*/
  // Do we have a color sensor?
  if (!tcs.begin()) {
    DEBUG_PRINTLN("No color sensor found.");
    while (1);
  } else {
    DEBUG_PRINTLN("Found the color sensor.");
  }

  testAllWheels();
  
  // Do we have a compass?
  if(!bno.begin()) {
    DEBUG_PRINTLN("No compass found.");
    while(1);
  } else {
    DEBUG_PRINTLN("Compass found. Calibrating...");
  }
  delay(500);
  bno.setExtCrystalUse(true);

  uint8_t system, gyro, accel, mag = 0;
  char status[5];
  while(mag != 3) {
    bno.getCalibration(&system, &gyro, &accel, &mag);
    sprintf(status,"CAL%i",mag);
    displayString(status);
    delay(100);
  }
  DEBUG_PRINTLN("The compass is set up.");
  delay(100);

  // All done.
  displayString("Good"); // Or "rEdY"
}
