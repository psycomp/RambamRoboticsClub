# Rambam RamBots LED Badge

- Assemble the board. The ATTiny chip is pre-programmed, but, you can...
  - Install Arduino IDE (1.8.* preferred).
  - Install [ATTiny84 core](https://cyberblogspot.com/how-to-install-attinycore-on-arduino-ide/).
  - Buy or build an ATTiny programmer. I use the [Tiny AVR Programmer](https://www.sparkfun.com/products/11801) from [Sparkfun](https://www.sparkfun.com/). An Arduino can be wired as a programmer.
  - Install the Charlieplex library in this repo, and load Examples->Charlieplex->Charlieplex
- Power the board. 

The power max for the chip is 5v, so two button cells are too much. Don't know if it will run on one cell. I can't find my assembled badge, and I never had a chance to redo the board with a diode to limit 6v -> 5v.

<img src="./Untitled.png" width="25%" height="25%"/>
