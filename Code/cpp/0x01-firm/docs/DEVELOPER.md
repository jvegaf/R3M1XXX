# Developer Documentation

## System Architecture

### Dual-Core Strategy (RP2040)
The firmware leverages the dual-core nature of the RP2040 to separate timing-critical communication from visual effects.

- **Core 0 (Main Core)**:
    - **Responsibility**: USB MIDI communication, I2C bus management, Input processing.
    - **Constraint**: Must never block. No `delay()`. Loop execution time determines MIDI latency.
    - **Components**: `PotManager`, `MatrixManager`, `EncoderManager`.

- **Core 1 (Visual Core)**:
    - **Responsibility**: LED animations (NeoPixels).
    - **Constraint**: Can block (e.g., `delay(20)` for 50FPS). Disabling interrupts for WS2812 timing here does not affect USB MIDI on Core 0.
    - **Components**: `Adafruit_NeoPixel`.

### Directory Structure
```
├── src/
│   ├── main.cpp          # Entry point, setup() and loop() for both cores
│   ├── MatrixManager.h   # HT16K33 driver wrapper
│   ├── PotManager.h      # ADS7830 driver wrapper
│   └── EncoderManager.h  # I2C Encoder driver
├── include/
│   └── config.h          # Global configuration (Pins, Addresses, MIDI map)
├── lib/                  # Private libraries
├── test/                 # Unit tests
└── platformio.ini        # Build configuration
```

## Build System
The project uses **PlatformIO**.

### Dependencies
Managed via `platformio.ini`:
- `Adafruit TinyUSB Library`: USB Stack.
- `MIDI Library`: MIDI protocol handling.
- `Adafruit NeoPixel`: LED control.
- `Adafruit ADS7830`: ADC driver.
- `simple ht16k33 library`: Matrix driver.

### Building and Uploading
1. **Build**: `pio run`
2. **Upload**: `pio run -t upload`
    - Requires RP2040 in Bootloader mode (Hold BOOTSEL while plugging in).

## Testing

### Unit Testing
Run tests with:
```bash
pio test
```
Tests are located in `test/` and use the Unity framework.

### Hardware Debugging
- **Serial Monitor**: The USB Serial port is active alongside MIDI. Use `Serial.print()` for debugging.
- **MIDI Monitor**: Use tools like `MIDI-OX` (Windows) or `aseqdump` (Linux) to verify MIDI output.

## Contributing
1. **Code Style**: Follow standard C++11/Arduino conventions.
2. **New Features**:
    - Create a new Manager class for new hardware.
    - Add configuration to `config.h`.
    - Instantiate in `main.cpp`.
3. **Pull Requests**: Ensure code compiles and existing tests pass.
