#include <Wire.h>
#include <HMC58X3.h>

HMC58X3 magn;
float initialHeading;

void setup(void) {
  Serial.begin(9600);
  Wire.begin();
  
  magn.init(false); // Dont set mode yet, we'll do that later on.
  magn.calibrate(2, 10); // Use gain 1=default, valid 0-7, 7 not recommended.
  magn.setMode(0);

  float fx,fy,fz;
  delay(10);
  magn.getValues(&fx,&fy,&fz);
  initialHeading = atan2(fy, fx);
  if(initialHeading < 0) {
    initialHeading += 2 * M_PI;
  }
  initialHeading = (initialHeading * 180/M_PI) - 180.0;
}

void loop() { 
  float fx,fy,fz;
  delay(10);
  magn.getValues(&fx,&fy,&fz);
  Serial.print(" Heading: ");
  float heading = atan2(fy, fx);
  if(heading < 0) {
    heading += 2 * M_PI;
  }
  heading = (heading * 180/M_PI) - 180.0;
  
  heading -= initialHeading;
  Serial.println(heading); 
}
