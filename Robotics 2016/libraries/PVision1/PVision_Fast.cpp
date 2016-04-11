// Example of using the PVision library for interaction with the Pixart sensor on a WiiMote
// This work was derived from Kako's excellent Japanese website
// http://www.kako.com/neta/2007-001/2007-001.html

// Steve Hobley 2009 - www.stephenhobley.com

// Steven Weinberger 2013 - updated to work with Arduino 1.0+

#include "Arduino.h"
#include "PVision_Fast.h"
#include <I2C.h>
#include <stdarg.h>
#include <stdio.h>

/******************************************************************************
* Constructor
******************************************************************************/
PVision_Fast::PVision_Fast() {
	Blob1.number = 1;
	Blob2.number = 2;
	Blob3.number = 3;
	Blob4.number = 4;
}

void PVision_Fast::init () {
	const uint8_t sequence1[]= { 0x90, 0x00, 0xC0 };
	const uint8_t sequence2[]= { 0x40, 0x00 };

	I2c.timeOut(100);
	I2c.pullup(0);
	I2c.setSpeed(1);
	I2c.begin();
	I2c.write(WIICAM_SLAVE_ADDRESS, 0x30,0x01); delay(10);
	I2c.write(WIICAM_SLAVE_ADDRESS, 0x30,0x08); delay(10);

	//I2c.write(WIICAM_SLAVE_ADDRESS, 0x06,const_cast<uint8_t*>(sequence1), sizeof(sequence1));
	I2c.write(WIICAM_SLAVE_ADDRESS, 0x06,0x90); delay(10);
	I2c.write(WIICAM_SLAVE_ADDRESS, 0x08,0xC0); delay(10);

	//I2c.write(WIICAM_SLAVE_ADDRESS, 0x1A,const_cast<uint8_t*>(sequence2), sizeof(sequence2));
	I2c.write(WIICAM_SLAVE_ADDRESS, 0x1A,0x40); delay(10);

	I2c.write(WIICAM_SLAVE_ADDRESS, 0x33,0x33); delay(100);
}

uint8_t PVision_Fast::read() {
	I2c.read(WIICAM_SLAVE_ADDRESS, 0x36, 16, data_buf);

    blobcount = 0;

    Blob1.X = data_buf[1];
    Blob1.Y = data_buf[2];
    s = data_buf[3];
    Blob1.X += (s & 0x30) <<4;
    Blob1.Y += (s & 0xC0) <<2;
    Blob1.Size = (s & 0x0F);

    blobcount |= (Blob1.Size < 15)? BLOB1 : 0;

    Blob2.X = data_buf[4];
    Blob2.Y = data_buf[5];
    s = data_buf[6];
    Blob2.X += (s & 0x30) <<4;
    Blob2.Y += (s & 0xC0) <<2;
    Blob2.Size = (s & 0x0F);

    blobcount |= (Blob2.Size < 15)? BLOB2 : 0;

    Blob3.X = data_buf[7];
    Blob3.Y = data_buf[8];
    s = data_buf[9];
    Blob3.X += (s & 0x30) <<4;
    Blob3.Y += (s & 0xC0) <<2;
    Blob3.Size = (s & 0x0F);

    blobcount |= (Blob3.Size < 15)? BLOB3 : 0;

    Blob4.X = data_buf[10];
    Blob4.Y = data_buf[11];
    s = data_buf[12];
    Blob4.X += (s & 0x30) <<4;
    Blob4.Y += (s & 0xC0) <<2;
    Blob4.Size = (s & 0x0F);

    blobcount |= (Blob4.Size < 15)? BLOB4 : 0;

    return blobcount;
}
