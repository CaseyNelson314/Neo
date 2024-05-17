#pragma once

#include <Udon.hpp>
#include <Adafruit_NeoPixel.h>
#include <cmath>

class Switch
{
    uint8_t pin;

public:
    Switch(uint8_t pin)
        : pin(pin)
    {}

    void begin()
    {
        pinMode(pin, INPUT_PULLUP);
    }

    bool press() const
    {
        return not digitalRead(pin);
    }

    bool unpress() const
    {
        return not press();
    }
};

class Volume
{
    uint8_t pin;

    Udon::MovingAverage<100> ma;

public:
    Volume(uint8_t pin)
        : pin(pin)
    {}

    uint16_t getVoltage()
    {
        return ma(analogRead(pin));
    }
};

class Lightbar
{

    // LED
    Adafruit_NeoPixel pixels;

    // 操作パネル
    Switch powerSwitch;
    Volume brightnessCtrl;  // 輝度変更用
    Volume colorTempCtrl;   // 色温度変更用

    int brightness{};

public:

    Lightbar(Adafruit_NeoPixel&& pixels, Switch&& powerSwitch, Volume&& brightnessCtrl, Volume&& colorTempCtrl)
        : pixels(pixels)
        , powerSwitch(powerSwitch)
        , brightnessCtrl(brightnessCtrl)
        , colorTempCtrl(colorTempCtrl)
    {}

    void begin()
    {
        pixels.begin();
        powerSwitch.begin();
    }

    void update()
    {
        if (powerSwitch.unpress())
        {
            // 電源 ON時

            if (brightness < 255)
            {
                brightness += 2;  // 緩やかに点灯
            }

            const auto kelvin     = Udon::Constrain(Udon::Map(colorTempCtrl .getVoltage(), 930, 50, 4000, 8000), 4000, 8000);
            const auto brightness = Udon::Constrain(Udon::Map(brightnessCtrl.getVoltage(), 930, 50,  0.2,  1.0),  0.2,  1.0);

            if (kelvin < 3100 and brightness < 0.21)
            {
                const auto rgb = Udon::HSV{ millis() / 3 % 255, 255, 255 }.toRGB();
                pixels.fill(rgb.to24bit());
            }
            else
            {
                const auto rgb = KelvinToRgb(kelvin) * brightness;
                pixels.fill(rgb.to24bit());
            }

        }
        else
        {
            // 電源 OFF時

            if (brightness > 0)
            {
                brightness -= 2;  // 緩やかに消灯
            }
        }

        pixels.setBrightness(Udon::Constrain(brightness, 0, 255));
        pixels.show();
    }


    /// @brief 色温度->RGB
    /// @link https://github.com/mattdesl/kelvin-to-rgb/blob/master/index.js
    static Udon::RGB KelvinToRgb(double kelvin)
    {
        double temp = kelvin / 100.0;
        double r, g, b;

        // 赤の計算
        if (temp <= 66.0) {
            r = 255;
        } else {
            r = temp - 60.0;
            r = 329.698727446 * std::pow(r, -0.1332047592);
            r = constrain(r, 0.0, 255.0);
        }

        // 緑の計算
        if (temp <= 66.0) {
            g = temp;
            g = 99.4708025861 * std::log(g) - 161.1195681661;
            g = constrain(g, 0.0, 255.0);
        } else {
            g = temp - 60.0;
            g = 288.1221695283 * std::pow(g, -0.0755148492);
            g = constrain(g, 0.0, 255.0);
        }

        // 青の計算
        if (temp >= 66.0) {
            b = 255;
        } else if (temp <= 19.0) {
            b = 0;
        } else {
            b = temp - 10.0;
            b = 138.5177312231 * std::log(b) - 305.0447927307;
            b = constrain(b, 0.0, 255.0);
        }

        // ガンマ補正を適用
        return {
            static_cast<uint8_t>(Adafruit_NeoPixel::gamma8(r)),
            static_cast<uint8_t>(Adafruit_NeoPixel::gamma8(g)),
            static_cast<uint8_t>(Adafruit_NeoPixel::gamma8(b)),
        };
    }

};
