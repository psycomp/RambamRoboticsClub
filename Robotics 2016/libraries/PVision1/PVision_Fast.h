// PVision library for interaction with the Pixart sensor on a WiiMote
// This work was derived from Kako's excellent Japanese website
// http://www.kako.com/neta/2007-001/2007-001.html

// Steve Hobley 2009 - www.stephenhobley.com

// Steven Weinberger 2013 - updated to work with Arduino 1.0+

#ifndef PVision_Fast_h
#define PVision_Fast_h

#include "Arduino.h"
#include <I2C.h>

// bit flags for blobs
#define BLOB1 0x01
#define BLOB2 0x02
#define BLOB3 0x04
#define BLOB4 0x08

#define WIICAM_SLAVE_ADDRESS 0x21

// Returned structure from a call to readSample()
struct Blob {
   	int X;
   	int Y;
   	int Size;
   	uint8_t number;
};

class PVision_Fast {

public:
  	PVision_Fast();

	void init();     // returns true if the connection to the sensor established correctly
	uint8_t read();  // updated the blobs, and returns the number of blobs detected

	Blob Blob1;
	Blob Blob2;
	Blob Blob3;
	Blob Blob4;

private:
	int i;
	int s;

	uint8_t data_buf[16];
	uint8_t blobcount; // returns the number of blobs found - reads the sensor
};


#endif
