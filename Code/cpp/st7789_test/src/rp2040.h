////////////////////////////////////////////////////////////////////////////////////////////
// Interface
////////////////////////////////////////////////////////////////////////////////////////////
#define TFT_PARALLEL_16_BIT

////////////////////////////////////////////////////////////////////////////////////////////
// Display driver type
////////////////////////////////////////////////////////////////////////////////////////////
#define ST7789_DRIVER

////////////////////////////////////////////////////////////////////////////////////////////
// RP2040 pins used
////////////////////////////////////////////////////////////////////////////////////////////

//#define TFT_CS -1 // Do not define, chip select control pin permanently connected to 0V

// These pins can be moved and are controlled directly by the library software
#define TFT_COUNT 2

//Definition of pin TFTsx-----------------------------
#define TFT_CS 17
#define TFT_RST 21
#define TFT_DC 20
#define TFT_MOSI 19
#define TFT_SCLK 18

//Definition of pin TFTdx----------------------------
//#define TFT1_CS 13
//#define TFT1_RST 15
//#define TFT1_DC 12
//#define TFT1_MOSI 11
//#define TFT1_SCLK 14

////////////////////////////////////////////////////////////////////////////////////////////
// Fonts to be available
////////////////////////////////////////////////////////////////////////////////////////////
#define LOAD_GLCD // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH
#define LOAD_FONT2 // Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH, 96 characters
#define LOAD_FONT4 // Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH, 96 characters
#define LOAD_FONT6 // Font 6. Large 48 pixel font, needs ~2666 bytes in FLASH, only characters 1234567890:-.apm
#define LOAD_FONT7 // Font 7. 7 segment 48 pixel font, needs ~2438 bytes in FLASH, only characters 1234567890:.
#define LOAD_FONT8 // Font 8. Large 75 pixel font needs ~3256 bytes in FLASH, only characters 1234567890:-.
#define LOAD_GFXFF // FreeFonts. Include access to the 48 Adafruit_GFX free fonts FF1 to FF48 and custom fonts

#define SMOOTH_FONT

////////////////////////////////////////////////////////////////////////////////////////////