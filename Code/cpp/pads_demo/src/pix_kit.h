#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

class PixKit {
private:
  uint8_t pin;        // Pin where the NeoPixel strip is connected
  uint8_t num_pixels; // Number of pixels in the strip
  Adafruit_NeoPixel *np_p;

public:
  PixKit(uint8_t p, uint8_t n) : pin(p), num_pixels(n) {
    np_p = new Adafruit_NeoPixel(num_pixels, pin, NEO_GRB + NEO_KHZ800);
  };
  ~PixKit() { delete np_p; }
  void begin();
  void setPixel(uint8_t n, uint32_t c);
  void show();
  uint32_t Color(uint8_t r, uint8_t g, uint8_t b);
  uint8_t numPixels();
  void setRainbow();
  void clear();

  const uint32_t CLEAR_COL = Adafruit_NeoPixel::Color(0, 0, 0);
  const uint32_t BLUE_COL = Adafruit_NeoPixel::Color(0, 0, 255);
  const uint32_t GREEN_COL = Adafruit_NeoPixel::Color(0, 255, 0);
  const uint32_t RED_COL = Adafruit_NeoPixel::Color(255, 0, 0);
  const uint32_t YELLOW_COL = Adafruit_NeoPixel::Color(255, 230, 0);
  const uint32_t ORANGE_COL = Adafruit_NeoPixel::Color(255, 143, 0);
  const uint32_t PURPLE_COL = Adafruit_NeoPixel::Color(243, 0, 255);
  const uint32_t GRAY_COL = Adafruit_NeoPixel::Color(191, 201, 202);
};
