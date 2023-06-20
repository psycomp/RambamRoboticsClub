#include <Adafruit_GFX.h>
#include "Charlieplex.h"

Charlieplex::Charlieplex() : Adafruit_GFX(4,5) {}

int Charlieplex::convert(int x, int y) {
  return(COLUMNS - (x + y) + (x * (COLUMNS + 1) + x));
}

int Charlieplex::convert16(int16_t x, int16_t y) {
  return(COLUMNS - (x + y) + (x * (COLUMNS + 1) + x));
}

void Charlieplex::drawPixel(int16_t x, int16_t y, uint16_t color) {
	_frame[convert16(x,y)] = 100;
}
void Charlieplex::erasePixel(int x, int y) { _frame[convert(x,y)] = 0; }

void Charlieplex::clearFrame(void){
 for(int pos=0;pos<=19;pos++) { _frame[pos] = 0; }
}

/*
 *   LED POS           ARRAY POS          CARTESIAN
 * 
 * 4  9  14  19  =>   0  1  2  3  => (0,0) (1,0) (2,0) (3,0)
 * 3  8  13  18  =>   4  5  6  7  => (0,1) (1,1) (2,1) (3,1)
 * 2  7  12  17  =>   8  9 10 11  => (0,2) (1,2) (2,2) (3,2)
 * 1  6  11  16  =>  12 13 14 15  => (0,3) (1,3) (2,3) (3,3)
 * 0  5  10  15  =>  16 17 18 19  => (0,4) (1,4) (2,4) (3,4)
 * 
 */

// shift left everything moves 5 spaces back. first 5 go to the last five
// shift right everything moves 5 spaces ahead. last 5 go to the beginning
// Shift up everything moves 1 space back except the top row. those substract 4
// shift down everthing moves 1 space ahead except the bottom row. those add 4.

void Charlieplex::shiftLeft(boolean rotate){ 
	int temp[5];
	if(rotate) {
	}
	if(rotate) {
	} else {
	}
}

void Charlieplex::shiftRight(boolean rotate) {
	int temp[5];
	if(rotate) {
	}
	if(rotate) {
	} else {
	}
}

void Charlieplex::shiftUp(boolean rotate){
	int temp[4];
	if(rotate) {
	}
	if(rotate) {
	} else {
	}
}

void Charlieplex::shiftDown(boolean rotate){
	int temp[4];
	if(rotate) {
	}
	if(rotate) {
	} else {
	}
}

void Charlieplex::setFrame(int frame[]) { 
 for(int pos=0;pos<=19;pos++) {
	_frame[convert(pos%COLUMNS, pos/COLUMNS)] = frame[pos];
 }
}
int *Charlieplex::getFrame(void){ return _frame; }

void Charlieplex::drawFrame(void){ drawFrame(_frame); }

void Charlieplex::drawFrame(int frame[]) {
 int led, bright_val, b;
 for ( led=0; led<=19; led++ ) {
  bright_val = frame[led];
  for( b=0 ; b < bright_val ; b+=4 ) { light_led(led); } //delay while on
  for( b=bright_val ; b<100 ; b+=4 ) { leds_off(); } //delay while off
 }
}

void Charlieplex::light_led(int led_num) { //led_num must be from 0 to 19
  DDRB = led_dir[led_num];
  PORTB = led_out[led_num];
}

void Charlieplex::leds_off() {
  DDRB = 0;
  PORTB = 0;
}
