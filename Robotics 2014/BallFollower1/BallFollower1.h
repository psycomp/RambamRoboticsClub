// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef _BallFollower1_H_
#define _BallFollower1_H_
#include "Arduino.h"
//add your includes for the project BallFollower1 here

#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <HTInfraredSeeker.h>

#include "HolonomicDrive.h"

#define NONE 0
#define LEFT 1
#define RIGHT 2

//end of add your includes here
#ifdef __cplusplus
extern "C" {
#endif
void loop();
void setup();
#ifdef __cplusplus
} // extern "C"
#endif

//add your function definitions for the project BallFollower1 here

extern Adafruit_MotorShield AFMS;

//Do not add code below this line
#endif /* _BallFollower1_H_ */
