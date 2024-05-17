#include "Lightbar.hpp"

static Udon::LoopCycleController loopCtrl{ 1000 };

static Lightbar lightbar {
    Adafruit_NeoPixel{ 90/*lednum*/, 9/*pin*/, NEO_GRB + NEO_KHZ800 },
    Switch{ 2 },
    Volume{ A1 },
    Volume{ A0 },
};

void setup()
{
    lightbar.begin();
}

void loop()
{
    lightbar.update();
    loopCtrl.update();
}
