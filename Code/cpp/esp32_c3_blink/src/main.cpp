#include <Arduino.h>
/*
  Blink

  Flashes a LED every second, repeatedly.

  The ESP32-C3 SuperMini has an on-board LED you can control. 
  It is attached to digital pin 8. 
  LED_BUILTIN is set to the correct LED pin.

  This example code is in the public domain.

  Adapted from:
  https://www.arduino.cc/en/Tutorial/BuiltInExamples/Blink
*/

// the LED is at pin  GPIO8
#define LED_BUILTIN 8

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_BUILTIN, LOW);  // turn the LED on (LOW is the voltage level)
  delay(50);                      // wait 50 ms
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED off by making the voltage HIGH
  delay(950);                      // wait 950 ms
}
