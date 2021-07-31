#ifndef LED_Bar_H
#define LED_Bar_H

#include <Arduino.h>

enum LedType {
    LED_TYPE_SHIFT  = 16,
    LED_MAX_COUNT   = 0 << LED_TYPE_SHIFT | 12*16,
    LED_NEW_DEVICE  = 0 << LED_TYPE_SHIFT | 12*16,
    LED_TYPE_MASK   = (1 << LED_TYPE_SHIFT) - 1,        //indicated there at most 65535 leds
};

namespace Origin {
    class LedDevice {
      protected:
        uint32_t pinClock;
        uint32_t pinData;
        uint32_t countOfShows;
        LedType  type;
        bool     reverseShow;
        int      numLeds;
        uint16_t  led[LED_MAX_COUNT];
      public:
        LedDevice(uint32_t pinClock, uint32_t pinData, bool reverseShow, int numLeds, LedType type);
        uint32_t countOfLed();
        void send(uint16_t bits);
        void send();
        void latch();
    };
}

class LED_Bar : Origin::LedDevice {
  public:
    LED_Bar(uint32_t pinClock, uint32_t pinData, bool greenToRed, int numLeds, LedType type) :
        Origin::LedDevice(pinClock, pinData, greenToRed, numLeds, type) {
    }
    void begin() {}
    void setLed(int ledNum, uint32_t r, uint32_t g, uint32_t b);
    void show();
};

#endif
