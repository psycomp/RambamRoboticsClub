#include "Thread.h"
#include "ThreadController.h"
#include <TimerOne.h>

#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <HTInfraredSeeker.h>
#include <HMC58X3.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
HMC58X3 magn;

class IRThread: public Thread {
  public:
	int dir;
	int dist;

	void run(){
		// Reads the analog pin, and saves it localy
                InfraredResult InfraredBall = InfraredSeeker::ReadAC();
                dir = ((int)InfraredBall.Direction - 5);
                dist = 255 - ((int)InfraredBall.Strength);
		runned();
	}
};

class HeadingThread: public Thread {
  public:
	int heading;

	void run(){
          float fx,fy,fz;
          delay(10);
          magn.getValues(&fx,&fy,&fz);
          float h = atan2(fy, fx);
          if(h < 0) { h += 2 * M_PI; }
          heading = (h * 180/M_PI); 
          runned();
	}
};

// Now, let's use our new class of Thread
IRThread irseeker = IRThread();
HeadingThread compass = HeadingThread();
ThreadController controller = ThreadController();
void timerCallback(){ controller.run(); }

void setup(){
	Serial.begin(9600);
        AFMS.begin();
        InfraredSeeker::Initialize();
        magn.init(false); // Dont set mode yet, we'll do that later on.
        magn.calibrate(1, 32); // Use gain 1=default, valid 0-7, 7 not recommended.
        magn.setMode(0);

Serial.println("Initialized");

	irseeker.setInterval(100);
	compass.setInterval(100);
	controller.add(&irseeker);
	controller.add(&compass);

	Timer1.initialize(20000);
	Timer1.attachInterrupt(timerCallback);
	Timer1.start();
}

void loop(){
  int dist = irseeker.dist;
  int dir = irseeker.dir;
  int heading = compass.heading;

  // DEBUG
  Serial.print("IR Thread: ");
  Serial.print(dir);
  Serial.print(", ");
  Serial.println(dist);
  Serial.print("Heading Thread: ");
  Serial.println(heading);
  Serial.println();
  
  return;
  
  if(dist < 180) {
    if(dist < 135 && (dir >= -1 && dir <= 1)) {
      switch(dir) {
        case -1:
          // Ball is left of center
          rDrive(345, 40, 0);
          delay(2000);
          rDrive(165, 40, 0);
          break;
        case 1:
          // Ball is right of center
          rDrive(15, 40, 0);
          delay(2000);
          rDrive(200, 40, 0);
          break;
        default:
          // Ball is straight ahead
          rDrive(0, 40, 0);
          delay(2000);
          rDrive(180, 40, 0);
          break;
      }
      delay(2000);
      halt();      
    } else {
      // Ball is close but not that close. Track it.
      if(dir < 0) {
          // It's off to the left.
          spin(-30);
      } else if(dir > 0) {
          // It's off to the right.
          spin(30);
      } else {
          // We are looking at it.
          halt(); 
      } 
    } 

  }
  
  delay(100);
}

