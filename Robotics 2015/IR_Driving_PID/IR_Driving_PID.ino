#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <SPI.h>  
#include <Pixy.h>
#include <PID_v1.h>


Adafruit_MotorShield AFMS = Adafruit_MotorShield();
int _stopMode = BRAKE; // BRAKE or RELEASE must be set.

double Setpoint, Input, Output;
PID myPID(&Input, &Output, &Setpoint,2,5,1, DIRECT);

Pixy pixy;
int minx, maxx, miny, maxy;
int cx, cy;
int ox, oy;

void setup() {
  Serial.begin(9600);
  AFMS.begin();
  pixy.init();
  Input = 100;
  Setpoint = 100;
  myPID.SetMode(AUTOMATIC);
}

void loop() { 
  static int i = 0;
  int j;
  uint16_t blocks;
  char buf[32]; 
  
  blocks = pixy.getBlocks();
  
  if (blocks) {
    i++;
    
      maxx = -999;
      maxy = -999;
      minx = 999;
      miny = 999;

      int x1, y1, x2, y2;

      for (j=0; j<blocks; j++) {
        x1 = pixy.blocks[j].x;
        x2 = x1 + pixy.blocks[j].width;
        y1 = pixy.blocks[j].y;
        y2 = y1 + pixy.blocks[j].height;

        if(x1 < minx) { minx = x1; }
        if(y1 < miny) { miny = y1; }
        if(x2 > maxx) { maxx = x2; }
        if(y2 > maxy) { maxy = y2; }
      }

      cx = x1 + ((x2 - x1) / 2); // 0-319
      cy = y1 + ((y2 - y1) / 2); // 0-199

Serial.print("Centroid: ");
Serial.println(cx);

      ox = cx - 160; // Right Positive, Left Negative
      oy = 100 - cy; // Up Positive, Down Negative

      /*
      if(oy > 0) { Serial.print("UP, "); }
      else { Serial.print("DOWN, "); }

      if(ox > 0) { Serial.println("RIGHT"); }
      else { Serial.println("LEFT"); }
      */

      Input = cx;
      myPID.Compute();
      //Serial.println(Output);
/*
      if(ox > 20) {
        Serial.println("Turn Right.");
        spin(20);
      }
      else if(ox < 20){
        Serial.println("Turn Left.");
        spin(-20);
      } else {
        Serial.println("Stop.");
        halt();
      }
*/
  }
}

