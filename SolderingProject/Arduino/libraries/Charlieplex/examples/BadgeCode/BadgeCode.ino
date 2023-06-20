#include "Charlieplex.h";
Charlieplex buffer1 = Charlieplex();
Charlieplex buffer2 = Charlieplex();

int current = 1;
unsigned long previousMillis = 0;
const long interval = 100;

int frame[] = {
  100,
  000,
  000,
  100,
  000,
  100,
  100,
  000,
  000,
  000,
  000,
  000,
  000,
  100,
  100,
  000,
  100,
  000,
  000,
  100,
};

void setup() {
  buffer1.drawRect(0, 0, 4, 5, 0);
  buffer2.drawRect(1, 1, 2, 3, 0);

  buffer1.setFrame(frame);
}

void loop() {

  buffer1.drawFrame();

  if (current == 1)
    buffer1.drawFrame();
  else
    buffer2.drawFrame();

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    if (current == 1) current = 2;
    else current = 1;
  }
}
