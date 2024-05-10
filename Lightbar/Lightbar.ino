#include "Lightbar.hpp"

static Udon::LoopCycleController loopCtrl{ 1000 };

static Lightbar lightbar {
    Adafruit_NeoPixel{ 56/*lednum*/, 11/*pin*/, NEO_GRB + NEO_KHZ800 },
    Switch{ 1 },
    Volume{ 2 },
    Volume{ 3 },
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
