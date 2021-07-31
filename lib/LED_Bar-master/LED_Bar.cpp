#ifdef _MSC_BUILD
    #include <stdint.h>
#endif

#include "LED_Bar.h"

enum LedState {
    LED_TURN_OFF,
    LED_FULL_BRIGHTNESS = 0xFF,
};

namespace Origin {
    LedDevice::LedDevice(uint32_t pinClock, uint32_t pinData, bool reverseShow, int numLeds, LedType type) {
        this->pinClock = pinClock;
        this->pinData = pinData;
        this->reverseShow = reverseShow;
        this->type = type;
        this->countOfShows = countOfLed();                                                     //number of leds
        this->numLeds = numLeds;
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
            //digitalWrite(pinData, bits & 0x8000 ? HIGH : LOW);                       //0x8000 is a mask for MSB
            if(bits & 0x8000) GPOS = (1 << pinData);
            else GPOC = (1 << pinData);
            if(clk) GPOS = (1 << pinClock);
            else GPOC = (1 << pinClock);

            clk = ~clk;
            bits <<= 1;
        }
    }

    void LedDevice::send() {
          //int cmd_bit = 0x710;
          //int cmd_bit = 0x310;
          //0B 00000000 00 0 00000
          int cmd_bit = 0B0000000011100000;
          //barra 2
          send(cmd_bit); //send cmd(0x00)
          for (uint32_t i = 12*16; i-- > 12*15;) send(led[i-6]); //16

          for(uint32_t j = 15; j > 8; j--){
            send(cmd_bit);
            for (uint32_t i = 12*j; i-- > 12*(j-1);) send(led[i-6]);
          }
          //barra 1
          for(uint32_t j = 8; j > 0; j--){
            send(cmd_bit);
            for (uint32_t i = 12*j; i-- > 12*(j-1);) send(led[i]);
          }
    }

    void LedDevice::latch() {
        digitalWrite(pinData, LOW);
        //for(uint32_t j = 0; j < 2; j++) GPOS = (1 << pinClock), GPOC = (1 << pinClock);
        digitalWrite(pinClock, HIGH); digitalWrite(pinClock, LOW);
        digitalWrite(pinClock, HIGH); digitalWrite(pinClock, LOW);
        delayMicroseconds(240);
        //for(uint32_t j = 0; j < 4; j++) GPOS = (1 << pinData), GPOC = (1 << pinData);
        digitalWrite(pinData, HIGH); digitalWrite(pinData, LOW);
        digitalWrite(pinData, HIGH); digitalWrite(pinData, LOW);
        digitalWrite(pinData, HIGH); digitalWrite(pinData, LOW);
        digitalWrite(pinData, HIGH); digitalWrite(pinData, LOW);
        delayMicroseconds(1);
        //for(uint32_t j = 0; j < 2; j++) GPOS = (1 << pinClock), GPOC = (1 << pinClock);
        digitalWrite(pinClock, HIGH);
        digitalWrite(pinClock, LOW);
    }
}

void LED_Bar::setLed(int ledNum, uint32_t r, uint32_t g, uint32_t b) {
    uint32_t r_pos = (ledNum-1)*3;
    uint32_t g_pos = (ledNum-1)*3+1;
    uint32_t b_pos = (ledNum-1)*3+2;

    led[r_pos] = r;
    led[g_pos] = g;
    led[b_pos] = b;
    send();
}

void LED_Bar::show() {
    latch();
}
