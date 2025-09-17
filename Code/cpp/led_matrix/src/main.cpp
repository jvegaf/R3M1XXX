#include "Adafruit_LEDBackpack.h"
#include <Adafruit_GFX.h>
#include <Arduino.h>
#include <Wire.h>

Adafruit_8x16matrix matrix = Adafruit_8x16matrix();
int DELAY_T = 1000;

void setup() {
  Serial.begin(9600);
  Serial.println("16x8 LED Matrix Test");

  matrix.begin(0x70); // pass in the address
  matrix.setBrightness(1);
}

static const uint8_t PROGMEM smile_bmp[] = {B00111100, B01000010, B10100101,
                                            B10000001, B10100101, B10011001,
                                            B01000010, B00111100},
                             neutral_bmp[] = {B00111100, B01000010, B10100101,
                                              B10000001, B10111101, B10000001,
                                              B01000010, B00111100},
                             frown_bmp[] = {B00111100, B01000010, B10100101,
                                            B10000001, B10011001, B10100101,
                                            B01000010, B00111100};

void loop() {

  matrix.clear();
  matrix.drawBitmap(0, 0, smile_bmp, 8, 8, LED_ON);
  matrix.writeDisplay();
  delay(DELAY_T);

  matrix.clear();
  matrix.drawBitmap(0, 8, neutral_bmp, 8, 8, LED_ON);
  matrix.writeDisplay();
  delay(DELAY_T);

  matrix.clear();
  matrix.drawBitmap(0, 0, frown_bmp, 8, 8, LED_ON);
  matrix.writeDisplay();
  delay(DELAY_T);

  matrix.clear(); // clear display
  matrix.drawPixel(0, 0, LED_ON);
  matrix.writeDisplay(); // write the changes we just made to the display
  delay(DELAY_T);

  matrix.clear();
  matrix.drawLine(0, 0, 7, 15, LED_ON);
  matrix.writeDisplay(); // write the changes we just made to the display
  delay(DELAY_T);

  matrix.clear();
  matrix.drawRect(0, 0, 8, 16, LED_ON);
  matrix.fillRect(2, 2, 4, 12, LED_ON);
  matrix.writeDisplay(); // write the changes we just made to the display
  delay(DELAY_T);

  matrix.clear();
  matrix.drawCircle(3, 8, 3, LED_ON);
  matrix.writeDisplay(); // write the changes we just made to the display
  delay(DELAY_T);

  matrix.setTextSize(2);
  matrix.setTextWrap(false); // we dont want text to wrap so it scrolls nicely
  matrix.setTextColor(LED_ON);
  for (int8_t x = 0; x >= -64; x--) {
    matrix.clear();
    matrix.setCursor(x, 0);
    matrix.print("Hello");
    matrix.writeDisplay();
    delay(DELAY_T);
  }

  matrix.setTextSize(1);
  matrix.setTextWrap(false); // we dont want text to wrap so it scrolls nicely
  matrix.setTextColor(LED_ON);
  matrix.setRotation(1);
  for (int8_t x = 7; x >= -36; x--) {
    matrix.clear();
    matrix.setCursor(x, 0);
    matrix.print("World");
    matrix.writeDisplay();
    delay(DELAY_T);
  }
  matrix.setRotation(0);
}
