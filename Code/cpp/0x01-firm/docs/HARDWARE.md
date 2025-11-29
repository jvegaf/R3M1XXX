# Hardware Documentation

## Microcontroller
- **Board**: Raspberry Pi Pico (RP2040)
- **Logic Voltage**: 3.3V
- **Clock Speed**: 133 MHz (Default)

## Pinout Map

| Function | RP2040 Pin | Physical Pin | Description |
|----------|------------|--------------|-------------|
| **I2C SDA** | GP4 | 6 | Main I2C Data Line |
| **I2C SCL** | GP5 | 7 | Main I2C Clock Line |
| **NeoPixel** | GP16 | 21 | Data for WS2812B LEDs |
| **USB** | - | - | Power & MIDI Data |

## I2C Bus Map
The system relies heavily on the I2C bus running at **1 MHz** (Fast Mode Plus).

| Address | Device | Chip | Function |
|---------|--------|------|----------|
| `0x30` | Encoder 1 | ATtiny412 | Rotary Encoder + Button |
| `0x31` | Encoder 2 | ATtiny412 | Rotary Encoder + Button |
| `0x32` | Encoder 3 | ATtiny412 | Rotary Encoder + Button |
| `0x48` | Potentiometers | ADS7830 | 8-Channel 8-bit ADC |
| `0x70` | Main Matrix | HT16K33 | 8x8 Matrix (Buttons/LEDs) |
| `0x71` | Display | HT16K33 | 7-Segment Display |
| `0x72` | Aux Matrix | HT16K33 | 16 Button Matrix |

### Wiring Notes
- **Pull-up Resistors**: The I2C bus requires pull-up resistors on SDA and SCL lines. 4.7kΩ to 3.3V is recommended.
- **Power**: All I2C devices in this design are compatible with 3.3V or 5V. The RP2040 logic is 3.3V. Ensure VCC for I2C devices matches or is compatible.
    - *ADS7830*: 3.3V or 5V (Ref voltage depends on VCC).
    - *HT16K33*: 3.3V or 5V.
    - *ATtiny412*: 3.3V or 5V.

## NeoPixels (WS2812B)
- **Count**: 16 LEDs
- **Power**: Connected to VBUS (5V) for maximum brightness.
- **Data**: Connected to GP16 (3.3V).
    - *Note*: While 3.3V logic often drives 5V WS2812Bs successfully, a level shifter (3.3V -> 5V) is recommended for reliability, especially with longer cables.

## Power Consumption
- **USB Current Limit**: Standard USB 2.0 is 500mA.
- **LEDs**: 16 NeoPixels at full white can draw ~960mA.
    - **Mitigation**: `LED_BRIGHTNESS` is set to 50 (approx 20%) in `config.h` to keep consumption safe for USB power.
