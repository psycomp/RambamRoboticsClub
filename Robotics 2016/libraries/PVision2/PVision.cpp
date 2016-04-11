// Example of using the PVision library for interaction with the Pixart sensor on a WiiMote
// This work was derived from Kako's excellent Japanese website
// http://www.kako.com/neta/2007-001/2007-001.html

// Steve Hobley 2009 - www.stephenhobley.com

// Steven Weinberger 2013 - updated to work with Arduino 1.0+

#include "Arduino.h"
#include "PVision.h"
#include <Wire.h>
#include <stdarg.h>
#include <stdio.h>

/******************************************************************************
* Private methods
******************************************************************************/
void PVision::Write_2bytes(uint8_t d1, uint8_t d2) {
    Wire.beginTransmission(IRslaveAddress);
    Wire.write(d1); Wire.write(d2);
    Wire.endTransmission();
}

void PVision::wrt(int i, ...) {
    va_list arguments;                     
    va_start(arguments, i);           

    Wire.beginTransmission(IRslaveAddress);
    for ( int x = 0; x < i; x++ ) {
        Wire.write(va_arg(arguments, int)); 
    }
    Wire.endTransmission();
    va_end(arguments);
}


/******************************************************************************
* Constructor
******************************************************************************/
PVision::PVision() {
	Blob1.number = 1;
	Blob2.number = 2;
	Blob3.number = 3;
	Blob4.number = 4;
}

/******************************************************************************
* Public methods
******************************************************************************/
void PVision::init () {
    IRsensorAddress = 0xB0;
    IRslaveAddress = IRsensorAddress >> 1;   // This results in 0x21 as the address to pass to TWI

    Wire.begin();
    // IR sensor initialize
    wrt(2, 0x30,0x01); delay(10);
    wrt(2, 0x30,0x08); delay(10);
    wrt(2, 0x06,0x90); delay(10);
    wrt(2, 0x08,0xC0); delay(10);
    wrt(2, 0x1A,0x40); delay(10);
    wrt(2, 0x33,0x33); delay(10);

/*
    Write_2bytes(0x30,0x01); delay(10);
    Write_2bytes(0x30,0x08); delay(10);
    Write_2bytes(0x06,0x90); delay(10);
    Write_2bytes(0x08,0xC0); delay(10);
    Write_2bytes(0x1A,0x40); delay(10);
    Write_2bytes(0x33,0x33); delay(10);
*/

    delay(100);
}

/************************************************************************

    FROM: http://makezine.com/2008/11/22/hacking-the-wiimote-ir-ca/

    Initialization with sensitivity setting:
    The author defined 5 sensitivity levels, and there are four parameters (p0, p1, p2, p3) that are adjusted for
    each level. Here are the settings:

    Level 1: p0 = 0x72, p1 = 0x20, p2 = 0x1F, p3 = 0x03
    Level 2: p0 = 0xC8, p1 = 0x36, p2 = 0x35, p3 = 0x03
    Level 3: p0 = 0xAA, p1 = 0x64, p2 = 0x63, p3 = 0x03
    Level 4: p0 = 0x96, p1 = 0xB4, p2 = 0xB3, p3 = 0x04
    Level 5: p0 = 0x96, p1 = 0xFE, p2 = 0xFE, p3 = 0x05

    These parameters correspond to:
    p0: MAXSIZE: Maximum blob size. Wii uses values from 0x62 to 0xc8
    p1: GAIN: Sensor Gain. Smaller values = higher gain
    p2: GAINLIMIT: Sensor Gain Limit. Must be less than GAIN for camera to function. No other effect?
    p3: MINSIZE: Minimum blob size. Wii uses values from 3 to 5

    Either pick your own custom settings for the parameters, or choose them from one of the 5 levels above,
    then send the following data to the device:

    0x30, 0x01
    0x00, 0x02, 0x00, 0x00, 0x71, 0x01, 0x00, p0
    0x07, 0x00, p1
    0x1A, p2, p3
    0x33, 0x03
    0x30, 0x08
    
************************************************************************/

void PVision::settings(uint8_t mode) {
    switch(mode) {
        case 1:
            specialInit(0x72, 0x20, 0x1F, 0x03);
            break;
        case 2:
            specialInit(0xC8, 0x36, 0x35, 0x03);
            break;
        case 3:
            specialInit(0xAA, 0x64, 0x63, 0x03);
            break;
        case 4:
            specialInit(0x96, 0xB4, 0xB3, 0x04);
            break;
        case 5:
            specialInit(0x96, 0xFE, 0xFE, 0x05);
            break;
    }
}

void PVision::specialInit(uint8_t p0, uint8_t p1, uint8_t p2, uint8_t p3) {
    wrt(2, 0x30, 0x01); delay(10);
    wrt(8, 0x00, 0x02, 0x00, 0x00, 0x71, 0x01, 0x00, p0); delay(10);
    wrt(3, 0x07, 0x00, p1); delay(10);
    wrt(3, 0x1A, p2, p3); delay(10);
    wrt(2, 0x33, 0x03); delay(10);
    wrt(2, 0x30, 0x08); delay(10);
    delay(100);
}

uint8_t PVision::read() {
    Wire.beginTransmission(IRslaveAddress);
    Wire.write(0x36);
    Wire.endTransmission();

    Wire.requestFrom(IRslaveAddress, 16);        // Request the 2 byte heading (MSB comes first)
    for (i=0;i<16;i++) {
       data_buf[i]=0;
    }

    i=0;
    while(Wire.available() && i < 16) {
        data_buf[i] = Wire.read();
        i++;
    }

    blobcount = 0;

    Blob1.X = data_buf[1];
    Blob1.Y = data_buf[2];
    s = data_buf[3];
    Blob1.X += (s & 0x30) <<4;
    Blob1.Y += (s & 0xC0) <<2;
    Blob1.Size = (s & 0x0F);

    // At the moment we're using the size of the blob to determine
    // if one is detected, either X,Y, or size could be used.

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
