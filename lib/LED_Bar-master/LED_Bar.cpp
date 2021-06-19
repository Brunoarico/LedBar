#ifdef _MSC_BUILD
    #include<stdint.h>
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
            digitalWrite(pinData, bits & 0x8000 ? HIGH : LOW);                       //0x8000 is a mask for MSB
            digitalWrite(pinClock, clk);
            clk = ~clk;
            bits <<= 1;
        }
    }

    /*void LedDevice::send() {
          int cmd_bit = 0x710;
          send(cmd_bit); //send cmd(0x00)
          for (uint32_t i = 12*16; i-- > 12*15;) send(led[i-6]); //16
          send(cmd_bit); //send cmd(0x00)
          for (uint32_t i = 12*15; i-- > 12*14;) send(led[i-6]); //15
          send(cmd_bit); //send cmd(0x00)
          for (uint32_t i = 12*14; i-- > 12*13;) send(led[i-6]); //14
          send(cmd_bit); //send cmd(0x00)
          for (uint32_t i = 12*13; i-- > 12*12;) send(led[i-6]); //13
          send(cmd_bit); //send cmd(0x00)
          for (uint32_t i = 12*12; i-- > 12*11;) send(led[i-6]); //12
          send(cmd_bit); //send cmd(0x00)
          for (uint32_t i = 12*11; i-- > 12*10;) send(led[i-6]); //11
          send(cmd_bit); //send cmd(0x00)
          for (uint32_t i = 12*10; i-- > 12*9;) send(led[i-6]); //10
          send(cmd_bit); //send cmd(0x00)
          for (uint32_t i = 12*9; i-- > 12*8;) send(led[i-6]); //9

          //barra 1
          send(cmd_bit); //send cmd(0x00)
          for (uint32_t i = 12*8; i-- > 12*7;) send(led[i]); //8
          send(cmd_bit); //send cmd(0x00)
          for (uint32_t i = 12*7; i-- > 12*6;) send(led[i]); //7
          send(cmd_bit); //send cmd(0x00)
          for (uint32_t i = 12*6; i-- > 12*5;) send(led[i]); //6
          send(cmd_bit); //send cmd(0x00)
          for (uint32_t i = 12*5; i-- > 12*4;) send(led[i]); //5
          send(cmd_bit); //send cmd(0x00)
          for (uint32_t i = 12*4; i-- > 12*3;) send(led[i]); //4
          send(cmd_bit); //send cmd(0x00)
          for (uint32_t i = 12*3; i-- > 12*2;) send(led[i]); //3
          send(cmd_bit); //send cmd(0x00)
          for (uint32_t i = 12*2; i-- > 12*1;) send(led[i]); //2
          send(cmd_bit); //send cmd(0x00)
          for (uint32_t i = 12*1; i-- > 12*0;) send(led[i]); //1
          //latch();
    }*/

    void LedDevice::send() {
          int cmd_bit = 0x710;
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

void LED_Bar::setLed(int ledNum, uint16_t r, uint16_t g, uint16_t b) {
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

void LED_Bar::show() {
    latch();
}
