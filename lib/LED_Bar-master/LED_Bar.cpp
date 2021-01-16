#ifdef _MSC_BUILD
    #include<stdint.h>
#endif

#include "LED_Bar.h"

enum LedState {
    LED_TURN_OFF,
    LED_FULL_BRIGHTNESS = 0xFF,
};

namespace Origin {
    LedDevice::LedDevice(uint32_t pinClock, uint32_t pinData, bool reverseShow, LedType type) {
        this->pinClock = pinClock;
        this->pinData = pinData;
        this->reverseShow = reverseShow;
        this->type = type;
        this->countOfShows = countOfLed();                                                     //number of leds

        for (uint32_t i = 0; i < LED_MAX_COUNT; i++) led[i] = LED_TURN_OFF;                    //Turn off all leds

        pinMode(pinClock, OUTPUT);
        pinMode(pinData, OUTPUT);
    }

    //Return the number of leds available
    uint32_t LedDevice::countOfLed() {
        return (uint32_t)type & LED_TYPE_MASK;
    }

    //send array of  bits serialy
    void LedDevice::send(uint16_t bits) {
        for (uint32_t i = 0, clk = 0; i < 16; i++) {                                 //16 bits per led and cmd
            digitalWrite(pinData, bits & 0x8000 ? HIGH : LOW);                       //0x8000 is a mask for MSB
            digitalWrite(pinClock, clk);
            clk = ~clk;
            bits <<= 1;
        }
    }

    void LedDevice::send() {
        if (reverseShow) {
            send(0x00); //send cmd(0x00)
            for (uint32_t i = 96; i-- > 84;) send(led[i]);
            send(0x00); //send cmd(0x00)
            for (uint32_t i = 84; i-- > 72;) send(led[i]);
            send(0x00); //send cmd(0x00)
            for (uint32_t i = 72; i-- > 60;) send(led[i]);
            send(0x00); //send cmd(0x00)
            for (uint32_t i = 60; i-- > 48;) send(led[i]);
            send(0x00); //send cmd(0x00)
            for (uint32_t i = 48; i-- > 36;) send(led[i]);
            send(0x00); //send cmd(0x00)
            for (uint32_t i = 36; i-- > 24;) send(led[i]);
            send(0x00); //send cmd(0x00)
            for (uint32_t i = 24; i-- > 12;) send(led[i]);
            send(0x00); //send cmd(0x00)
            for (uint32_t i = 12; i-- > 0;) send(led[i]);
        }

        else {
            send(0x00);
            for (uint32_t i = 0; i < 12; i++) send(led[i]);
            send(0x00);
            for (uint32_t i = 12; i < 24; i++) send(led[i]);
        }

        latch();
    }

    void LedDevice::latch() {
        digitalWrite(pinData, LOW);
        digitalWrite(pinClock, HIGH); digitalWrite(pinClock, LOW);
        digitalWrite(pinClock, HIGH); digitalWrite(pinClock, LOW);
        delayMicroseconds(240);
        digitalWrite(pinData, HIGH); digitalWrite(pinData, LOW);
        digitalWrite(pinData, HIGH); digitalWrite(pinData, LOW);
        digitalWrite(pinData, HIGH); digitalWrite(pinData, LOW);
        digitalWrite(pinData, HIGH); digitalWrite(pinData, LOW);
        delayMicroseconds(1);
        digitalWrite(pinClock, HIGH);
        digitalWrite(pinClock, LOW);
    }
}

void LED_Bar::setLed (int ledNum, uint16_t r, uint16_t g, uint16_t b) {
    uint8_t r_pos = (ledNum-1)*3;
    uint8_t g_pos = (ledNum-1)*3+1;
    uint8_t b_pos = (ledNum-1)*3+2;
    led[r_pos] = r;
    led[g_pos] = g;
    led[b_pos] = b;
    send();
}

void LED_Bar::toggleLed(uint32_t ledNo) {
    uint32_t i =
        ledNo <= countOfShows ?
        ledNo - 1 :
        countOfShows - 1;
    led[i] = led[i] ? LED_TURN_OFF : LED_FULL_BRIGHTNESS;
    send();
}
