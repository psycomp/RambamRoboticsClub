// ---------------------------------------------------------------------------
// Example NewPing library sketch that does a ping about 20 times per second.
// ---------------------------------------------------------------------------

#include <NewPing.h>

NewPing sonar2(2, 3); // NewPing setup of pins and maximum distance.
NewPing sonar1(4, 5); // NewPing setup of pins and maximum distance.

void setup() {
  Serial.begin(9600); // Open serial monitor at 115200 baud to see ping results.
}

void loop() {
  delay(50);                      // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  unsigned int uS1 = sonar1.ping(); // Send ping, get ping time in microseconds (uS).
  unsigned int uS2 = sonar2.ping(); // Send ping, get ping time in microseconds (uS).
  Serial.print("Ping: ");
  Serial.print(uS1 / US_ROUNDTRIP_CM); // Convert ping time to distance in cm and print result (0 = outside set distance range)
  Serial.print("cm, ");
  Serial.print(uS2 / US_ROUNDTRIP_CM); // Convert ping time to distance in cm and print result (0 = outside set distance range)
  Serial.println("cm");
}
