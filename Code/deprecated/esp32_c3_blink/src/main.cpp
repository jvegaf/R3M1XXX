#include <Arduino.h>

// the LED is at pin  GPIO8
#define LED_BUILTIN 8
#define SLEEP_TIME 200

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_BUILTIN, LOW); // turn the LED on (LOW is the voltage level)
  delay(SLEEP_TIME);              // wait 50 ms
  digitalWrite(LED_BUILTIN,
               HIGH); // turn the LED off by making the voltage HIGH
  delay(SLEEP_TIME);  // wait 950 ms
}
