/****************************************************************
 * HT16K33 LED Matrix and Keyboard Position Identifier
 *
 * Advanced diagnostic tool for mapping physical positions of LEDs
 * and keyboard buttons on HT16K33-based devices.
 *
 * Features:
 * - Comprehensive LED mapping with multiple test patterns
 * - Detailed keyboard mapping with raw data display
 * - Interactive menu system for mode selection
 * - Individual LED/key testing capability
 * - Visual grid representation of LED states
 * - Hardware timing and position debugging information
 *
 * Hardware Info:
 * - IC: HT16K33 LED Driver with Key Scan
 * - LED Matrix: 16 rows × 8 columns = 128 LEDs
 * - Keyboard: 3 groups × 13 keys = 39 keys max
 * - I2C Base Address: 0x70 (configurable 0x70-0x77)
 *
 * Author: Embedded Systems Test Tool
 * Version: 2.0
 ****************************************************************/
#include "ht16k33.h"
#include <Arduino.h>

/****************************************************************
 * Hardware Configuration Constants
 ****************************************************************/
constexpr uint8_t LED_COUNT = 128;     // 16 rows × 8 columns
constexpr uint8_t LED_ROWS = 16;       // Physical LED matrix rows
constexpr uint8_t LED_COLS = 8;        // Physical LED matrix columns
constexpr uint8_t KEY_COUNT = 39;      // 3 groups × 13 keys
constexpr uint8_t KEY_GROUPS = 3;      // Number of key groups
constexpr uint8_t KEYS_PER_GROUP = 13; // Keys in each group

/****************************************************************
 * Timing Configuration Constants
 ****************************************************************/
constexpr uint16_t LED_DELAY_MS = 300;      // Delay between LED tests
constexpr uint16_t LED_FAST_DELAY_MS = 100; // Fast LED test delay
constexpr uint16_t SCAN_DELAY_MS = 50;      // Keyboard scan rate
constexpr uint16_t DEBOUNCE_MS = 200;       // Key debounce delay
constexpr uint32_t MENU_TIMEOUT_MS = 60000; // Menu timeout (1 minute)

// HT16K33 controller instance
HT16K33 HT;

/****************************************************************
 * Operating Modes
 ****************************************************************/
enum class TestMode : uint8_t {
  MENU,           // Main menu
  LED_SEQUENTIAL, // Test all LEDs sequentially
  LED_BY_ROW,     // Test LEDs row by row
  LED_BY_COLUMN,  // Test LEDs column by column
  LED_PATTERN,    // Test specific patterns
  LED_INDIVIDUAL, // Test individual LED
  LED_MANUAL,     // Manual LED control with j/k keys
  KEY_MAPPING,    // Keyboard mapping test
  KEY_RAW_DATA,   // Show raw keyboard data
  INTERACTIVE,    // Combined interactive mode
  LED_GRID_VIEW   // Show current LED state grid
};

TestMode currentMode = TestMode::MENU;

/****************************************************************
 * Display Main Menu
 ****************************************************************/
void displayMenu() {
  Serial.println(F("\n╔═══════════════════════════════════════════╗"));
  Serial.println(F("║       HT16K33 Diagnostic Menu             ║"));
  Serial.println(F("╠═══════════════════════════════════════════╣"));
  Serial.println(F("║  LED Tests:                               ║"));
  Serial.println(F("║    1 - Sequential LED Test (all 128)      ║"));
  Serial.println(F("║    2 - Test LEDs by Row                   ║"));
  Serial.println(F("║    3 - Test LEDs by Column                ║"));
  Serial.println(F("║    4 - Pattern Test (checkerboard, etc)   ║"));
  Serial.println(F("║    5 - Individual LED Test                ║"));
  Serial.println(F("║    6 - Manual LED Control (j/k keys)      ║"));
  Serial.println(F("║    7 - LED Grid View                      ║"));
  Serial.println(F("║                                           ║"));
  Serial.println(F("║  Keyboard Tests:                          ║"));
  Serial.println(F("║    8 - Keyboard Mapping                   ║"));
  Serial.println(F("║    9 - Raw Keyboard Data                  ║"));
  Serial.println(F("║                                           ║"));
  Serial.println(F("║  Combined:                                ║"));
  Serial.println(F("║    i - Interactive Mode (LED + Keys)      ║"));
  Serial.println(F("║    0 - Show this menu                     ║"));
  Serial.println(F("╚═══════════════════════════════════════════╝"));
  Serial.println(F("Enter choice: "));
}

/****************************************************************
 * LED Position Information Display
 * Shows detailed information about LED addressing
 ****************************************************************/
void displayLedInfo(uint8_t ledNum) {
  uint8_t row = ledNum / LED_COLS;
  uint8_t col = ledNum % LED_COLS;
  uint8_t ramByte = ledNum / 8;
  uint8_t ramBit = ledNum % 8;

  Serial.print(F("LED #"));
  if (ledNum < 100)
    Serial.print(F(" "));
  if (ledNum < 10)
    Serial.print(F(" "));
  Serial.print(ledNum);
  Serial.print(F(" | Row: "));
  if (row < 10)
    Serial.print(F(" "));
  Serial.print(row);
  Serial.print(F(", Col: "));
  Serial.print(col);
  Serial.print(F(" | RAM["));
  if (ramByte < 10)
    Serial.print(F(" "));
  Serial.print(ramByte);
  Serial.print(F("]:bit"));
  Serial.print(ramBit);
  Serial.print(F(" | 0x"));
  Serial.print(1 << ramBit, HEX);
  Serial.println();
}

/****************************************************************
 * Key Press Information Display
 * Shows which key was pressed with state information
 ****************************************************************/
void displayKeyInfo(int8_t keyValue) {
  if (keyValue > 0) {
    uint8_t keyNum = static_cast<uint8_t>(keyValue);
    uint8_t group = (keyNum - 1) / KEYS_PER_GROUP;
    uint8_t keyInGroup = (keyNum - 1) % KEYS_PER_GROUP;

    Serial.print(F("KEY PRESSED: #"));
    if (keyNum < 10)
      Serial.print(F(" "));
    Serial.print(keyNum);
    Serial.print(F(" | Group: "));
    Serial.print(group);
    Serial.print(F(", Position: "));
    if (keyInGroup < 10)
      Serial.print(F(" "));
    Serial.print(keyInGroup);
    Serial.println(F(" | [PRESSED]"));
  } else if (keyValue < 0) {
    uint8_t keyNum = static_cast<uint8_t>(-keyValue);
    Serial.print(F("KEY RELEASED: #"));
    if (keyNum < 10)
      Serial.print(F(" "));
    Serial.print(keyNum);
    Serial.println(F(" | [RELEASED]"));
  }
}

/****************************************************************
 * Display Raw Keyboard Data
 * Shows the raw 3×16-bit keyboard scan data
 ****************************************************************/
void displayKeyRaw() {
  HT16K33::KEYDATA keyData;
  HT.readKeyRaw(keyData, true);

  Serial.println(F("\n=== Raw Keyboard Data ==="));
  for (uint8_t group = 0; group < KEY_GROUPS; group++) {
    Serial.print(F("Group "));
    Serial.print(group);
    Serial.print(F(": 0x"));
    if (keyData[group] < 0x1000)
      Serial.print(F("0"));
    if (keyData[group] < 0x100)
      Serial.print(F("0"));
    if (keyData[group] < 0x10)
      Serial.print(F("0"));
    Serial.print(keyData[group], HEX);
    Serial.print(F(" | Binary: "));

    // Display binary representation
    for (int8_t bit = 12; bit >= 0; bit--) {
      Serial.print((keyData[group] >> bit) & 1);
      if (bit % 4 == 0 && bit != 0)
        Serial.print(F(" "));
    }
    Serial.println();
  }
  Serial.println();
}

/****************************************************************
 * LED Matrix Sequential Test
 * Tests all LEDs sequentially to identify positions
 ****************************************************************/
void testLedSequential() {
  Serial.println(F("\n╔═══════════════════════════════════════════╗"));
  Serial.println(F("║     LED Sequential Position Test          ║"));
  Serial.println(F("╚═══════════════════════════════════════════╝"));
  Serial.println(F("Testing all 128 LEDs sequentially..."));
  Serial.println(F("Format: LED# | Row, Col | RAM[byte]:bit | Bitmask\n"));

  for (uint8_t led = 0; led < LED_COUNT; led++) {
    HT.setLedNow(led);
    displayLedInfo(led);
    delay(LED_DELAY_MS);
    HT.clearLedNow(led);

    // Check for serial input to abort
    if (Serial.available()) {
      Serial.read();
      Serial.println(F("\n[ABORTED] Test stopped by user."));
      break;
    }
  }

  Serial.println(F("\n=== Sequential Test Complete ===\n"));
  delay(1000);
}

/****************************************************************
 * LED Matrix Row Test
 * Tests LEDs row by row
 ****************************************************************/
void testLedByRow() {
  Serial.println(F("\n╔═══════════════════════════════════════════╗"));
  Serial.println(F("║         LED Row-by-Row Test               ║"));
  Serial.println(F("╚═══════════════════════════════════════════╝"));

  for (uint8_t row = 0; row < LED_ROWS; row++) {
    Serial.print(F("\nTesting Row "));
    Serial.print(row);
    Serial.println(F(":"));

    // Light up entire row
    for (uint8_t col = 0; col < LED_COLS; col++) {
      uint8_t ledNum = row * LED_COLS + col;
      HT.setLed(ledNum);
    }
    HT.sendLed();

    // Display info for each LED in row
    for (uint8_t col = 0; col < LED_COLS; col++) {
      uint8_t ledNum = row * LED_COLS + col;
      displayLedInfo(ledNum);
    }

    delay(LED_DELAY_MS * 2);
    HT.clearAll();

    if (Serial.available()) {
      Serial.read();
      Serial.println(F("\n[ABORTED] Test stopped by user."));
      break;
    }
  }

  Serial.println(F("\n=== Row Test Complete ===\n"));
  delay(1000);
}

/****************************************************************
 * LED Matrix Column Test
 * Tests LEDs column by column
 ****************************************************************/
void testLedByColumn() {
  Serial.println(F("\n╔═══════════════════════════════════════════╗"));
  Serial.println(F("║       LED Column-by-Column Test           ║"));
  Serial.println(F("╚═══════════════════════════════════════════╝"));

  for (uint8_t col = 0; col < LED_COLS; col++) {
    Serial.print(F("\nTesting Column "));
    Serial.print(col);
    Serial.println(F(":"));

    // Light up entire column
    for (uint8_t row = 0; row < LED_ROWS; row++) {
      uint8_t ledNum = row * LED_COLS + col;
      HT.setLed(ledNum);
    }
    HT.sendLed();

    // Display info for each LED in column
    for (uint8_t row = 0; row < LED_ROWS; row++) {
      uint8_t ledNum = row * LED_COLS + col;
      displayLedInfo(ledNum);
    }

    delay(LED_DELAY_MS * 2);
    HT.clearAll();

    if (Serial.available()) {
      Serial.read();
      Serial.println(F("\n[ABORTED] Test stopped by user."));
      break;
    }
  }

  Serial.println(F("\n=== Column Test Complete ===\n"));
  delay(1000);
}

/****************************************************************
 * LED Pattern Test
 * Tests various patterns to verify LED mapping
 ****************************************************************/
void testLedPatterns() {
  Serial.println(F("\n╔═══════════════════════════════════════════╗"));
  Serial.println(F("║          LED Pattern Test                 ║"));
  Serial.println(F("╚═══════════════════════════════════════════╝"));

  // Pattern 1: Checkerboard
  Serial.println(F("\nPattern 1: Checkerboard"));
  for (uint8_t led = 0; led < LED_COUNT; led++) {
    if ((led / LED_COLS + led % LED_COLS) % 2 == 0) {
      HT.setLed(led);
    }
  }
  HT.sendLed();
  delay(2000);
  HT.clearAll();

  // Pattern 2: Inverse Checkerboard
  Serial.println(F("Pattern 2: Inverse Checkerboard"));
  for (uint8_t led = 0; led < LED_COUNT; led++) {
    if ((led / LED_COLS + led % LED_COLS) % 2 == 1) {
      HT.setLed(led);
    }
  }
  HT.sendLed();
  delay(2000);
  HT.clearAll();

  // Pattern 3: Diagonal stripes
  Serial.println(F("Pattern 3: Diagonal Stripes"));
  for (uint8_t led = 0; led < LED_COUNT; led++) {
    if ((led / LED_COLS + led % LED_COLS) % 4 < 2) {
      HT.setLed(led);
    }
  }
  HT.sendLed();
  delay(2000);
  HT.clearAll();

  // Pattern 4: Border
  Serial.println(F("Pattern 4: Border Frame"));
  for (uint8_t col = 0; col < LED_COLS; col++) {
    HT.setLed(col);                             // Top row
    HT.setLed((LED_ROWS - 1) * LED_COLS + col); // Bottom row
  }
  for (uint8_t row = 1; row < LED_ROWS - 1; row++) {
    HT.setLed(row * LED_COLS);                // Left column
    HT.setLed(row * LED_COLS + LED_COLS - 1); // Right column
  }
  HT.sendLed();
  delay(2000);
  HT.clearAll();

  Serial.println(F("\n=== Pattern Test Complete ===\n"));
  delay(1000);
}

/****************************************************************
 * Manual LED Control
 * Control LED position with j (forward) and k (backward) keys
 ****************************************************************/
void testLedManual() {
  Serial.println(F("\n╔═══════════════════════════════════════════╗"));
  Serial.println(F("║       Manual LED Control                  ║"));
  Serial.println(F("╠═══════════════════════════════════════════╣"));
  Serial.println(F("║  Controls:                                ║"));
  Serial.println(F("║    j - Next LED (forward)                 ║"));
  Serial.println(F("║    k - Previous LED (backward)            ║"));
  Serial.println(F("║    Any other key - Exit to menu           ║"));
  Serial.println(F("╚═══════════════════════════════════════════╝"));

  uint8_t currentLed = 0;
  bool exitRequested = false;

  // Display initial LED
  HT.clearAll();
  HT.setLedNow(currentLed);
  displayLedInfo(currentLed);

  while (!exitRequested) {
    if (Serial.available()) {
      char ch = Serial.read();

      switch (ch) {
      case 'j':
      case 'J':
        // Advance to next LED
        HT.clearLedNow(currentLed);
        currentLed = (currentLed + 1) % LED_COUNT; // Wrap around at 127
        HT.setLedNow(currentLed);
        displayLedInfo(currentLed);
        break;

      case 'k':
      case 'K':
        // Go to previous LED
        HT.clearLedNow(currentLed);
        if (currentLed == 0) {
          currentLed = LED_COUNT - 1; // Wrap to 127
        } else {
          currentLed--;
        }
        HT.setLedNow(currentLed);
        displayLedInfo(currentLed);
        break;

      default:
        // Any other key exits
        Serial.println(F("\n[EXITING] Returning to menu..."));
        HT.clearAll();
        exitRequested = true;
        break;
      }
    }
  }

  delay(500);
}

/****************************************************************
 * Individual LED Test
 * Test a specific LED by number
 ****************************************************************/
void testIndividualLed() {
  Serial.println(F("\n╔═══════════════════════════════════════════╗"));
  Serial.println(F("║        Individual LED Test                ║"));
  Serial.println(F("╚═══════════════════════════════════════════╝"));
  Serial.println(F("Enter LED number (0-127) or 255 to exit:"));

  while (true) {
    if (Serial.available()) {
      int ledNum = Serial.parseInt();

      if (ledNum == 255) {
        Serial.println(F("Exiting individual LED test..."));
        break;
      }

      if (ledNum >= 0 && ledNum < LED_COUNT) {
        HT.clearAll();
        HT.setLedNow(static_cast<uint8_t>(ledNum));
        displayLedInfo(static_cast<uint8_t>(ledNum));
        Serial.println(F("LED is ON. Enter another number or 255 to exit:"));
      } else {
        Serial.println(F("Invalid LED number! Enter 0-127 or 255 to exit:"));
      }
    }
    delay(50);
  }

  HT.clearAll();
  delay(1000);
}

/****************************************************************
 * Keyboard Mapping Test
 * Waits for key presses and displays their mapping
 ****************************************************************/
void testKeyMapping() {
  Serial.println(F("\n╔═══════════════════════════════════════════╗"));
  Serial.println(F("║        Keyboard Mapping Test              ║"));
  Serial.println(F("╚═══════════════════════════════════════════╝"));
  Serial.println(F("Press keys to see their mapping..."));
  Serial.println(F("Send any serial data to exit\n"));

  int8_t lastKey = 0;

  while (!Serial.available()) {
    int8_t key = HT.readKey();

    if (key != 0 && key != lastKey) {
      displayKeyInfo(key);
      lastKey = key;
    } else if (key == 0) {
      lastKey = 0;
    }

    delay(SCAN_DELAY_MS);
  }

  Serial.read(); // Clear serial buffer
  Serial.println(F("\n=== Keyboard Mapping Test Complete ===\n"));
  delay(1000);
}

/****************************************************************
 * Raw Keyboard Data Test
 * Continuously displays raw keyboard scan data
 ****************************************************************/
void testKeyRawData() {
  Serial.println(F("\n╔═══════════════════════════════════════════╗"));
  Serial.println(F("║       Raw Keyboard Data Monitor           ║"));
  Serial.println(F("╚═══════════════════════════════════════════╝"));
  Serial.println(F("Monitoring raw keyboard data..."));
  Serial.println(F("Send any serial data to exit\n"));

  while (!Serial.available()) {
    displayKeyRaw();
    delay(500);
  }

  Serial.read(); // Clear serial buffer
  Serial.println(F("\n=== Raw Data Monitoring Complete ===\n"));
  delay(1000);
}

/****************************************************************
 * Interactive Mode
 * Combined LED test with keyboard interaction
 ****************************************************************/
void interactiveMode() {
  Serial.println(F("\n╔═══════════════════════════════════════════╗"));
  Serial.println(F("║          Interactive Mode                 ║"));
  Serial.println(F("╚═══════════════════════════════════════════╝"));
  Serial.println(F("LEDs will cycle sequentially."));
  Serial.println(F("Press any key to pause/resume."));
  Serial.println(F("Send serial data to exit.\n"));

  bool paused = false;

  for (uint8_t led = 0; led < LED_COUNT; led++) {
    if (Serial.available()) {
      Serial.read();
      Serial.println(F("\n[EXITING] Interactive mode..."));
      break;
    }

    if (!paused) {
      HT.setLedNow(led);
      displayLedInfo(led);
      delay(LED_DELAY_MS);
      HT.clearLedNow(led);
    }

    // Check for key press
    int8_t key = HT.readKey();
    if (key > 0) {
      paused = !paused;
      if (paused) {
        Serial.println(F("\n[PAUSED] Press any key to resume..."));
        HT.setLedNow(led); // Keep current LED on
      } else {
        Serial.println(F("[RESUMED]"));
        HT.clearLedNow(led);
      }
      displayKeyInfo(key);
      delay(DEBOUNCE_MS);
    }
  }

  HT.clearAll();
  Serial.println(F("\n=== Interactive Mode Complete ===\n"));
  delay(1000);
}

/****************************************************************
 * Display LED State Grid
 * Shows which LEDs are currently on in a visual grid
 ****************************************************************/
void displayLedGrid() {
  Serial.println(F("\n╔═══════════════════════════════════════════╗"));
  Serial.println(F("║        Current LED State Grid             ║"));
  Serial.println(F("╚═══════════════════════════════════════════╝"));
  Serial.print(F("    "));

  // Column headers
  for (uint8_t col = 0; col < LED_COLS; col++) {
    Serial.print(col);
    Serial.print(F(" "));
  }
  Serial.println();
  Serial.println(F("    ----------------"));

  // Display grid with row numbers
  for (uint8_t row = 0; row < LED_ROWS; row++) {
    if (row < 10)
      Serial.print(F(" "));
    Serial.print(row);
    Serial.print(F("| "));

    for (uint8_t col = 0; col < LED_COLS; col++) {
      uint8_t ledNum = row * LED_COLS + col;
      if (HT.getLed(ledNum, true)) {
        Serial.print(F("█ ")); // LED is on
      } else {
        Serial.print(F("· ")); // LED is off
      }
    }
    Serial.println();
  }
  Serial.println();
}

/****************************************************************
 * Setup - Initialize hardware and display menu
 ****************************************************************/
void setup() {
  Serial.begin(57600);
  while (!Serial && millis() < 3000) {
    ; // Wait for serial connection (max 3 seconds)
  }

  Serial.println(F("\n\n"));
  Serial.println(F("╔════════════════════════════════════════════╗"));
  Serial.println(F("║  HT16K33 LED & Keyboard Position Mapper   ║"));
  Serial.println(F("║              Version 2.0                   ║"));
  Serial.println(F("╚════════════════════════════════════════════╝"));
  Serial.println();

  // Initialize HT16K33 (I2C address 0x00, becomes 0x70)
  Serial.println(F("Initializing HT16K33..."));
  HT.begin(0x01);
  Serial.println(F("✓ HT16K33 initialized successfully"));
  Serial.println();

  // Display hardware information
  Serial.println(F("Hardware Configuration:"));
  Serial.print(F("  • LED Matrix: "));
  Serial.print(LED_ROWS);
  Serial.print(F(" rows × "));
  Serial.print(LED_COLS);
  Serial.print(F(" cols = "));
  Serial.print(LED_COUNT);
  Serial.println(F(" LEDs"));
  Serial.print(F("  • Keyboard: "));
  Serial.print(KEY_COUNT);
  Serial.println(F(" keys (3 groups × 13)"));
  Serial.print(F("  • I2C Address: 0x"));
  Serial.println(0x70, HEX);
  Serial.println();

  // Quick flash test to verify all LEDs work
  Serial.println(F("Running quick flash test..."));
  for (uint8_t led = 0; led < LED_COUNT; led++) {
    HT.setLed(led);
  }
  HT.sendLed();
  delay(500);
  HT.clearAll();
  Serial.println(F("✓ Flash test complete\n"));

  delay(1000);

  // Display main menu
  displayMenu();
}

/****************************************************************
 * Main Loop - Interactive LED and Keyboard Testing
 ****************************************************************/
void loop() {
  // Handle menu selection
  if (currentMode == TestMode::MENU) {
    if (Serial.available()) {
      char choice = Serial.read();

      // Clear any extra characters
      while (Serial.available()) {
        Serial.read();
      }

      Serial.print(F("\nSelected: "));
      Serial.println(choice);

      switch (choice) {
      case '1':
        currentMode = TestMode::LED_SEQUENTIAL;
        break;
      case '2':
        currentMode = TestMode::LED_BY_ROW;
        break;
      case '3':
        currentMode = TestMode::LED_BY_COLUMN;
        break;
      case '4':
        currentMode = TestMode::LED_PATTERN;
        break;
      case '5':
        currentMode = TestMode::LED_INDIVIDUAL;
        break;
      case '6':
        currentMode = TestMode::LED_MANUAL;
        break;
      case '7':
        currentMode = TestMode::LED_GRID_VIEW;
        break;
      case '8':
        currentMode = TestMode::KEY_MAPPING;
        break;
      case '9':
        currentMode = TestMode::KEY_RAW_DATA;
        break;
      case 'i':
      case 'I':
        currentMode = TestMode::INTERACTIVE;
        break;
      case '0':
        displayMenu();
        break;
      default:
        Serial.println(F("Invalid choice! Please enter 0-9 or i."));
        displayMenu();
        break;
      }
    }
  }

  // Execute selected test mode
  switch (currentMode) {
  case TestMode::LED_SEQUENTIAL:
    testLedSequential();
    currentMode = TestMode::MENU;
    displayMenu();
    break;

  case TestMode::LED_BY_ROW:
    testLedByRow();
    currentMode = TestMode::MENU;
    displayMenu();
    break;

  case TestMode::LED_BY_COLUMN:
    testLedByColumn();
    currentMode = TestMode::MENU;
    displayMenu();
    break;

  case TestMode::LED_PATTERN:
    testLedPatterns();
    currentMode = TestMode::MENU;
    displayMenu();
    break;

  case TestMode::LED_INDIVIDUAL:
    testIndividualLed();
    currentMode = TestMode::MENU;
    displayMenu();
    break;

  case TestMode::LED_MANUAL:
    testLedManual();
    currentMode = TestMode::MENU;
    displayMenu();
    break;

  case TestMode::LED_GRID_VIEW:
    displayLedGrid();
    currentMode = TestMode::MENU;
    displayMenu();
    break;

  case TestMode::KEY_MAPPING:
    testKeyMapping();
    currentMode = TestMode::MENU;
    displayMenu();
    break;

  case TestMode::KEY_RAW_DATA:
    testKeyRawData();
    currentMode = TestMode::MENU;
    displayMenu();
    break;

  case TestMode::INTERACTIVE:
    interactiveMode();
    currentMode = TestMode::MENU;
    displayMenu();
    break;

  case TestMode::MENU:
    // Waiting for input, do nothing
    delay(100);
    break;
  }
}
