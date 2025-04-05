/****************************************************************
 * Simple test of ht16k33 library both led and keyboard
 */
#include <Arduino.h>
#include "ht16k33.h"

// Define the class
HT16K33 HT;


/****************************************************************/
void setup() {
  uint8_t led;
  Serial.begin(9600);
  Serial.println("ht16k33 keyboard and led test v0.01");
  Serial.println();
  // initialize everything, 0x00 is the i2c address for the first chip (0x70 is added in the class).
  HT.begin(0x00);

}

/****************************************************************/
void loop() {
  static uint8_t lastKey=0;
  int8_t key;

  delay(30);
  key=HT.readKey();
  if (key != 0) { // key is pressed
    if (key != lastKey){
      if(key<0) {
        Serial.print(F("Key released: "));
        Serial.println(-key);
      } else {
        Serial.print(F("Key pressed: "));
        Serial.println(key);
      }
      // Serial.print(F("Key pressed: "));
      // Serial.println(key);
      lastKey=key;
    }
  }
} // loop
