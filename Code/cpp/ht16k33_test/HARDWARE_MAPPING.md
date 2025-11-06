# HT16K33 Hardware Mapping Reference

## LED Matrix Architecture

### Physical Layout
```
        Column →
Row ↓   0   1   2   3   4   5   6   7
  0     0   1   2   3   4   5   6   7
  1     8   9  10  11  12  13  14  15
  2    16  17  18  19  20  21  22  23
  3    24  25  26  27  28  29  30  31
  4    32  33  34  35  36  37  38  39
  5    40  41  42  43  44  45  46  47
  6    48  49  50  51  52  53  54  55
  7    56  57  58  59  60  61  62  63
  8    64  65  66  67  68  69  70  71
  9    72  73  74  75  76  77  78  79
 10    80  81  82  83  84  85  86  87
 11    88  89  90  91  92  93  94  95
 12    96  97  98  99 100 101 102 103
 13   104 105 106 107 108 109 110 111
 14   112 113 114 115 116 117 118 119
 15   120 121 122 123 124 125 126 127
```

### Memory Mapping

#### Display RAM Organization
El HT16K33 tiene 16 bytes de display RAM, cada byte controla 8 LEDs:

```
Display RAM Address | LEDs Controlados | Row
--------------------|------------------|-----
0x00 (RAM[0])      | LEDs 0-7         | 0
0x01 (RAM[1])      | LEDs 8-15        | 1
0x02 (RAM[2])      | LEDs 16-23       | 2
0x03 (RAM[3])      | LEDs 24-31       | 3
0x04 (RAM[4])      | LEDs 32-39       | 4
0x05 (RAM[5])      | LEDs 40-47       | 5
0x06 (RAM[6])      | LEDs 48-55       | 6
0x07 (RAM[7])      | LEDs 56-63       | 7
0x08 (RAM[8])      | LEDs 64-71       | 8
0x09 (RAM[9])      | LEDs 72-79       | 9
0x0A (RAM[10])     | LEDs 80-87       | 10
0x0B (RAM[11])     | LEDs 88-95       | 11
0x0C (RAM[12])     | LEDs 96-103      | 12
0x0D (RAM[13])     | LEDs 104-111     | 13
0x0E (RAM[14])     | LEDs 112-119     | 14
0x0F (RAM[15])     | LEDs 120-127     | 15
```

#### Bit Mapping dentro de cada byte
```
Bit Position | Column | Bitmask
-------------|--------|--------
bit 0        | Col 0  | 0x01
bit 1        | Col 1  | 0x02
bit 2        | Col 2  | 0x04
bit 3        | Col 3  | 0x08
bit 4        | Col 4  | 0x10
bit 5        | Col 5  | 0x20
bit 6        | Col 6  | 0x40
bit 7        | Col 7  | 0x80
```

### Cálculo de Posiciones

#### LED Number → Row/Column
```cpp
uint8_t ledNumber = 45;
uint8_t row = ledNumber / 8;        // 45 / 8 = 5
uint8_t column = ledNumber % 8;     // 45 % 8 = 5
// LED 45 está en Row 5, Column 5
```

#### Row/Column → LED Number
```cpp
uint8_t row = 5;
uint8_t column = 5;
uint8_t ledNumber = row * 8 + column;  // 5 * 8 + 5 = 45
```

#### LED Number → RAM Address y Bit
```cpp
uint8_t ledNumber = 45;
uint8_t ramAddress = ledNumber / 8;    // 45 / 8 = 5 (RAM[5])
uint8_t bitPosition = ledNumber % 8;   // 45 % 8 = 5 (bit 5)
uint8_t bitmask = 1 << bitPosition;    // 1 << 5 = 0x20
```

### Ejemplos de Mapeo

| LED # | Row | Col | RAM Byte | Bit | Bitmask | Binario    |
|-------|-----|-----|----------|-----|---------|------------|
| 0     | 0   | 0   | 0        | 0   | 0x01    | 0000 0001  |
| 7     | 0   | 7   | 0        | 7   | 0x80    | 1000 0000  |
| 8     | 1   | 0   | 1        | 0   | 0x01    | 0000 0001  |
| 15    | 1   | 7   | 1        | 7   | 0x80    | 1000 0000  |
| 64    | 8   | 0   | 8        | 0   | 0x01    | 0000 0001  |
| 127   | 15  | 7   | 15       | 7   | 0x80    | 1000 0000  |

## Keyboard Matrix Architecture

### Keyboard Scan Structure

El HT16K33 soporta hasta **39 teclas** organizadas en 3 grupos de 13 teclas cada uno:

```
Key Number | Group | Position in Group | Key Data Register
-----------|-------|-------------------|------------------
1-13       | 0     | 0-12             | KEYDATA[0]
14-26      | 1     | 0-12             | KEYDATA[1]
27-39      | 2     | 0-12             | KEYDATA[2]
```

### Key Data Registers

Cada grupo tiene un registro de 16 bits (aunque solo 13 bits se usan):

```
KEYDATA[0] (Group 0): [15:13 unused] [12:0 Keys 13-1]
KEYDATA[1] (Group 1): [15:13 unused] [12:0 Keys 26-14]
KEYDATA[2] (Group 2): [15:13 unused] [12:0 Keys 39-27]
```

### Bit Mapping para Teclas

#### Group 0 (Keys 1-13)
```
Bit  | 12  11  10   9   8   7   6   5   4   3   2   1   0
Key  | 13  12  11  10   9   8   7   6   5   4   3   2   1
```

#### Group 1 (Keys 14-26)
```
Bit  | 12  11  10   9   8   7   6   5   4   3   2   1   0
Key  | 26  25  24  23  22  21  20  19  18  17  16  15  14
```

#### Group 2 (Keys 27-39)
```
Bit  | 12  11  10   9   8   7   6   5   4   3   2   1   0
Key  | 39  38  37  36  35  34  33  32  31  30  29  28  27
```

### Cálculo de Posiciones de Teclas

#### Key Number → Group/Position
```cpp
uint8_t keyNumber = 17;  // Range: 1-39
uint8_t group = (keyNumber - 1) / 13;           // (17-1)/13 = 1
uint8_t positionInGroup = (keyNumber - 1) % 13; // (17-1)%13 = 3
// Key 17 está en Group 1, Position 3
```

#### Group/Position → Key Number
```cpp
uint8_t group = 1;
uint8_t positionInGroup = 3;
uint8_t keyNumber = group * 13 + positionInGroup + 1; // 1*13 + 3 + 1 = 17
```

### Lectura de Datos Raw del Teclado

```cpp
HT16K33::KEYDATA keyData;
HT.readKeyRaw(keyData, true);  // Lee los 3 registros de 16 bits

// Ejemplo de salida:
// keyData[0] = 0x0008 significa que Key 4 está presionada (bit 3 set)
// keyData[1] = 0x0040 significa que Key 21 está presionada (bit 6 set)
// keyData[2] = 0x1000 significa que Key 39 está presionada (bit 12 set)
```

### Ejemplos de Mapeo de Teclas

| Key # | Group | Position | Bit en Register | Bitmask | Hex    |
|-------|-------|----------|----------------|---------|--------|
| 1     | 0     | 0        | 0              | 0x0001  | 0x0001 |
| 7     | 0     | 6        | 6              | 0x0040  | 0x0040 |
| 13    | 0     | 12       | 12             | 0x1000  | 0x1000 |
| 14    | 1     | 0        | 0              | 0x0001  | 0x0001 |
| 20    | 1     | 6        | 6              | 0x0040  | 0x0040 |
| 26    | 1     | 12       | 12             | 0x1000  | 0x1000 |
| 27    | 2     | 0        | 0              | 0x0001  | 0x0001 |
| 33    | 2     | 6        | 6              | 0x0040  | 0x0040 |
| 39    | 2     | 12       | 12             | 0x1000  | 0x1000 |

## HT16K33 Pin Assignments

### LED Drive Pins
```
Pin Name | Function           | Connected To
---------|-------------------|------------------
COM0-7   | Common cathode    | LED rows (COM)
ROW0-15  | LED row driver    | LED cathode/anode
```

### Key Scan Pins
```
Pin Name | Function          | Connected To
---------|------------------|------------------
K0-K12   | Key scan input   | Key matrix rows
S0-S2    | Key scan output  | Key matrix columns
```

### I2C Interface
```
Pin Name | Function    | Connected To
---------|------------|------------------
SDA      | I2C Data   | Microcontroller SDA
SCL      | I2C Clock  | Microcontroller SCL
```

### Address Selection
```
Pin Name | State | I2C Address
---------|-------|------------
A0, A1, A2 = GND | 0b000 | 0x70
A0=VDD, A1=A2=GND | 0b001 | 0x71
A1=VDD, A0=A2=GND | 0b010 | 0x72
A0=A1=VDD, A2=GND | 0b011 | 0x73
A2=VDD, A0=A1=GND | 0b100 | 0x74
A0=A2=VDD, A1=GND | 0b101 | 0x75
A1=A2=VDD, A0=GND | 0b110 | 0x76
A0=A1=A2=VDD | 0b111 | 0x77
```

## I2C Commands

### System Setup
```
Command     | Value | Function
------------|-------|------------------
SS_STANDBY  | 0x20  | Oscillator OFF
SS_NORMAL   | 0x21  | Oscillator ON
```

### Display Setup
```
Command       | Value | Function
--------------|-------|------------------
DSP_OFF       | 0x80  | Display OFF
DSP_ON        | 0x81  | Display ON
DSP_NOBLINK   | 0x81  | No blinking
DSP_BLINK2HZ  | 0x83  | Blink at 2Hz
DSP_BLINK1HZ  | 0x85  | Blink at 1Hz
DSP_BLINK05HZ | 0x87  | Blink at 0.5Hz
```

### Brightness Control
```
Command | Value Range | Function
--------|-------------|------------------
DIM     | 0xE0-0xEF   | Set brightness 1-16
```

Example:
- `0xE0`: Brightness 1/16 (dimmest)
- `0xE7`: Brightness 8/16 (50%)
- `0xEF`: Brightness 16/16 (brightest)

## Timing Considerations

### I2C Speed
- Standard mode: 100 kHz
- Fast mode: 400 kHz (typical)

### Key Scan Rate
- Recommended: 20-50ms between scans
- Debounce time: 200ms minimum for mechanical switches

### LED Refresh
- Automatic multiplexing by HT16K33
- Refresh rate: ~100Hz typical

## Power Consumption

### LED Current
- Max per LED: 40mA
- Total max: Limited by VDD current capability
- Set via external resistor on REXT pin

### Chip Current
- Sleep mode: ~10µA
- Active (no LEDs): ~2mA
- Active (all LEDs on): depends on LED current setting

## Practical Usage Tips

### For LED Identification
1. Start with sequential test (Option 1)
2. Note which physical LED lights up for each number
3. Create a mapping table for your specific hardware
4. Use row/column tests to verify matrix structure

### For Key Identification
1. Use keyboard mapping test (Option 7)
2. Press each physical key one at a time
3. Note the key number reported
4. Use raw data view (Option 8) to debug issues
5. Verify all 39 keys are accessible

### For Hardware Debugging
1. Use pattern tests to verify all LEDs work
2. Check raw keyboard data to see if matrix is responsive
3. Use grid view to verify LED states match expectations
4. Test individual LEDs to isolate problems

## Common Issues and Solutions

### Some LEDs don't light
- Check row/column connections
- Verify RAM byte is being written correctly
- Test with pattern that should light those LEDs

### Keys not responding
- Check pull-ups on key matrix
- Verify key matrix wiring
- Use raw data view to see if any bits change

### Intermittent behavior
- Check I2C pull-up resistors (4.7kΩ typical)
- Verify power supply is stable
- Add decoupling capacitor near HT16K33

### Wrong LEDs lighting
- Verify row/column connections match expectations
- Check if device has non-standard LED matrix wiring
- Use this tool to create custom mapping table

---

**Reference**: Based on HT16K33 datasheet and simple ht16k33 library implementation
