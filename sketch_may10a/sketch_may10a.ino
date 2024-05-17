#include <Adafruit_NeoPixel.h>

static Adafruit_NeoPixel pixels(90/*lednum*/, 9/*pin*/, NEO_GRB + NEO_KHZ800);

void setup()
{
    pixels.begin();
}

void loop() {
    for (uint16_t i = 0; i < pixels.numPixels(); ++i)
    {
        pixels.setPixelColor(i,0xffdddd);
    }
    pixels.show();
    delay(10);
}
