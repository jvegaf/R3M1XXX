# API Documentation

## Overview
This firmware is built around a modular architecture where each hardware component is managed by a dedicated manager class. These classes handle hardware abstraction, state management, and MIDI communication.

## Classes

### `MatrixManager`
Handles the HT16K33-based button and LED matrices.

**Header:** `src/MatrixManager.h`

#### Constructor
```cpp
MatrixManager(uint8_t addr, uint8_t startNote)
```
- **addr**: I2C address of the HT16K33 chip (e.g., `0x70`).
- **startNote**: The MIDI note number corresponding to the first button (index 0). Subsequent buttons use `startNote + index`.

#### Methods

##### `void begin()`
Initializes the HT16K33 chip, turns on the oscillator, and sets default brightness.
- **Usage**: Call in `setup()`.

##### `void updateInput(midi::MidiInterface<...> &midi)`
Reads the button states from the matrix and sends MIDI Note On/Off messages if changes are detected.
- **midi**: Reference to the global MIDI interface object.
- **Usage**: Call in `loop()`.

##### `void setLed(uint8_t ledIndex, bool state)`
Controls an individual LED on the matrix.
- **ledIndex**: Index of the LED (0-127, though typically 0-15 for this project).
- **state**: `true` for ON, `false` for OFF.
- **Note**: Updates are sent immediately to the chip.

---

### `PotManager`
Handles the ADS7830 8-channel ADC for potentiometers.

**Header:** `src/PotManager.h`

#### Methods

##### `bool begin()`
Initializes the ADS7830 ADC on the I2C bus.
- **Returns**: `true` if successful, `false` otherwise.
- **Usage**: Call in `setup()`.

##### `void update(midi::MidiInterface<...> &midi)`
Reads all 8 analog channels, converts values to 7-bit MIDI (0-127), and sends Control Change (CC) messages if values change.
- **midi**: Reference to the global MIDI interface object.
- **Features**: Implements hysteresis to prevent MIDI jitter.
- **Usage**: Call in `loop()`.

---

### `EncoderManager`
Handles I2C-based Rotary Encoders (ATtiny412 firmware).

**Header:** `src/EncoderManager.h`

#### Constructor
```cpp
EncoderManager(uint8_t addr, uint8_t cc, uint8_t note)
```
- **addr**: I2C address of the encoder module (e.g., `0x30`).
- **cc**: MIDI Control Change number for rotation events.
- **note**: MIDI Note number for the push-button.

#### Methods

##### `bool begin()`
Checks connection to the encoder module and initializes internal state.
- **Returns**: `true` if device is found on I2C bus.
- **Usage**: Call in `setup()`.

##### `void update(midi::MidiInterface<...> &midi)`
Reads encoder delta and button state via I2C.
- **midi**: Reference to the global MIDI interface object.
- **Behavior**:
    - **Rotation**: Sends relative MIDI CC values. `65` for CW (+1), `63` for CCW (-1).
    - **Button**: Sends Note On/Off messages.
- **Usage**: Call in `loop()`.

---

## Global Configuration
Configuration constants are defined in `include/config.h`.

| Constant | Description | Default |
|----------|-------------|---------|
| `MIDI_CH` | Global MIDI Channel | 1 |
| `CC_POTS_START` | Starting CC for Potentiometers | 10 |
| `CC_ENC_START` | Starting CC for Encoders | 20 |
| `NOTE_MAIN_START` | Starting Note for Main Matrix | 36 (C1) |
| `NOTE_AUX_START` | Starting Note for Aux Matrix | 52 (E2) |
| `NOTE_ENC_START` | Starting Note for Encoder Buttons | 68 |
