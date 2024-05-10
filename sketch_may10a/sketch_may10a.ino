#include <Adafruit_NeoPixel.h>

static Adafruit_NeoPixel pixels(10/*lednum*/, 11/*pin*/, NEO_GRB + NEO_KHZ800);

void setup() {
  pixels.begin();
}

void loop() {
    for (int i = 0; i < 10; ++i)
    {
        pixels.setPixelColor(i,0xffffff);
    }
    pixels.show();
    delay(10);
}
