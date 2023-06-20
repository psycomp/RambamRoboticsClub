#ifndef Charlieplex_h
#define Charlieplex_h

#include <Adafruit_GFX.h>

#define LINE_A 0 //Pin 5 (PB0) on ATtiny85
#define LINE_B 1 //Pin 6 (PB1) on ATtiny85
#define LINE_C 2 //Pin 7 (PB2) on ATtiny85
#define LINE_D 3 //Pin 2 (PB3) on ATtiny85
#define LINE_E 4 //Pin 3 (PB4) on ATtiny85
#define ROWS 5
#define COLUMNS 4

class Charlieplex : public Adafruit_GFX {
  public:
    Charlieplex();

	void drawPixel(int16_t x, int16_t y, uint16_t color);
	void erasePixel(int x, int y);
	void drawFrame(void);
	void drawFrame(int frame[]);
	void clearFrame(void);
	int *getFrame(void);
	void setFrame(int frame[]);

	void shiftUp(boolean rotate);
	void shiftDown(boolean rotate);
	void shiftRight(boolean rotate);
	void shiftLeft(boolean rotate);

  private:
	//DDRB direction config for each LED (1 = output)
	const char led_dir[20] = {
	  ( 1 << LINE_A | 1 << LINE_E ), //LED 0
	  ( 1 << LINE_B | 1 << LINE_E ), //LED 1
	  ( 1 << LINE_C | 1 << LINE_E ), //LED 2
	  ( 1 << LINE_D | 1 << LINE_E ), //LED 3
	  ( 1 << LINE_E | 1 << LINE_D ), //LED 4
	  ( 1 << LINE_A | 1 << LINE_D ), //LED 5
	  ( 1 << LINE_B | 1 << LINE_D ), //LED 6
	  ( 1 << LINE_C | 1 << LINE_D ), //LED 7
	  ( 1 << LINE_D | 1 << LINE_C ), //LED 8
	  ( 1 << LINE_E | 1 << LINE_C ), //LED 9
	  ( 1 << LINE_A | 1 << LINE_C ), //LED 10
	  ( 1 << LINE_B | 1 << LINE_C ), //LED 11
	  ( 1 << LINE_C | 1 << LINE_B ), //LED 12
	  ( 1 << LINE_D | 1 << LINE_B ), //LED 13
	  ( 1 << LINE_E | 1 << LINE_B ), //LED 14
	  ( 1 << LINE_A | 1 << LINE_B ), //LED 15
	  ( 1 << LINE_B | 1 << LINE_A ), //LED 16
	  ( 1 << LINE_C | 1 << LINE_A ), //LED 17
	  ( 1 << LINE_D | 1 << LINE_A ), //LED 18
	  ( 1 << LINE_E | 1 << LINE_A ) //LED 19
	};

	//PORTB output config for each LED (1 = High, 0 = Low)
	const char led_out[20] = {
	  ( 1 << LINE_A ), //LED 0
	  ( 1 << LINE_B ), //LED 1
	  ( 1 << LINE_C ), //LED 2
	  ( 1 << LINE_D ), //LED 3
	  ( 1 << LINE_E ), //LED 4
	  ( 1 << LINE_A ), //LED 5
	  ( 1 << LINE_B ), //LED 6
	  ( 1 << LINE_C ), //LED 7
	  ( 1 << LINE_D ), //LED 8
	  ( 1 << LINE_E ), //LED 9
	  ( 1 << LINE_A ), //LED 10
	  ( 1 << LINE_B ), //LED 11
	  ( 1 << LINE_C ), //LED 12
	  ( 1 << LINE_D ), //LED 13
	  ( 1 << LINE_E ), //LED 14
	  ( 1 << LINE_A ), //LED 15
	  ( 1 << LINE_B ), //LED 16
	  ( 1 << LINE_C ), //LED 17
	  ( 1 << LINE_D ), //LED 18
	  ( 1 << LINE_E ) //LED 19
	};

	int _frame[20] = {
	  0 , 0 , 0 , 0 ,
	  0 , 0 , 0 , 0 ,
	  0 , 0 , 0 , 0 ,
	  0 , 0 , 0 , 0 ,
	  0 , 0 , 0 , 0 ,
	};

	int convert(int x, int y);
	int convert16(int16_t x, int16_t y);
	void light_led(int led_num);
	void leds_off(void);

};

#endif
