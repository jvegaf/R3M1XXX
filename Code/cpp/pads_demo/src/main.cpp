#include "base.h"
#include "ht16k33.h"
#include "pix_kit.h"
#include <Arduino.h>
#include <MIDI.h>

MIDI_CREATE_DEFAULT_INSTANCE();

PixKit pixels(PIXEL_PIN, NUM_PIXELS);
HT16K33 HT;

volatile unsigned long lastRead = 0;
volatile uint8_t lastKey = 0;
volatile unsigned long lastMidiActivity = 0;

void handleNoteOn(uint8_t channel, uint8_t number, uint8_t value);
void handleNoteOff(uint8_t channel, uint8_t number, uint8_t value);
void midiSetup();
void readPad();

void setup() {
  // Serial.begin(115200);
  // while (!Serial) {
  //   ; // wait for serial port to connect. Needed for native USB port only
  // }
  // Serial.println("Begin");
  pixels.begin();
  HT.begin(HT16K33_ADDRESS);
  midiSetup();
  lastRead = millis();
}

void loop() {
  unsigned long currentTime = millis();
  if (lastRead + READ_INTERVAL_MS < currentTime) {
    readPad();
  }
  if (MIDI.read()) {
    lastMidiActivity = currentTime;
  }
  // Check for MIDI timeout (optional health check)
  if (currentTime - lastMidiActivity > MIDI_TIMEOUT_MS) {
    // Could implement reconnection logic here if needed
    pixels.setAll(0); // Clear all pixels on timeout
    midiSetup();      // Reinitialize MIDI to recover from potential issues
  }
}

void handleNoteOn(uint8_t channel, uint8_t number, uint8_t value) {
  // Validate velocity
  if (value > MAX_MIDI_VELOCITY) {
    value = MAX_MIDI_VELOCITY;
  }

  lastMidiActivity = millis();
  pixels.setPixel(number - MIDI_NOTE_MIN, value); // Map MIDI note to pixel
}

void handleNoteOff(uint8_t channel, uint8_t number, uint8_t value) {
  lastMidiActivity = millis();
  pixels.setPixel(number - MIDI_NOTE_MIN, 0); // Map MIDI note to pixel
}

void midiSetup() {
  // Configure MIDI handlers
  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);

  // Start MIDI with optimized settings
  MIDI.begin(MIDI_CHANNEL);
  MIDI.turnThruOff();

  // Set serial baud rate for better performance
  // Serial.begin(MIDI_BAUD_RATE);

  // Initialize timing variables
  lastMidiActivity = millis();
}

void readPad() {
  int8_t key;
  key = HT.readKey();
  if (key != 0) { // key is pressed
    if (key != lastKey) {
      if (key < 0) {
        uint8_t keyIdx = getBtnIndex(-key);
        MIDI.sendNoteOff(keyIdx + MIDI_NOTE_MIN, 0, MIDI_CHANNEL);
        // Serial.print(F("Key released: "));
        // Serial.println(-key);
      } else {
        uint8_t keyIdx = getBtnIndex(key);
        MIDI.sendNoteOn(keyIdx + MIDI_NOTE_MIN, 127, MIDI_CHANNEL);
        // Serial.print(F("Key pressed: "));
        // Serial.println(key);
      }
      lastKey = key;
    }
  }
  lastRead = millis();
}
