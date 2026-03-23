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
  void setPixel(uint8_t n, uint8_t c);
  void setAll(uint8_t c);
  uint32_t Color(uint8_t r, uint8_t g, uint8_t b);
  void setRainbow();
  void clear();
};
