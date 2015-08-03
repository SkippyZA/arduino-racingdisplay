//
// Created by Steven Inskip on 2015/06/24.
//

#ifndef SIMCONSOLE_DFROBOT7SEG_H
#define SIMCONSOLE_DFROBOT7SEG_H

#include <Arduino.h>
#include <WString.h>

class DFRobot7Seg {
public:
    DFRobot7Seg(uint8_t clock, uint8_t latch, uint8_t data) {
        clockPin = clock;
        latchPin = latch;
        dataPin = data;

        pinMode(latchPin, OUTPUT);
        pinMode(dataPin, OUTPUT);
        pinMode(clockPin, OUTPUT);
    }

    void print(const char* data);
    void print(String data);

    

private:
    uint8_t clockPin;
    uint8_t latchPin;
    uint8_t dataPin;

private:
    // 0, 1, 2,3 ,4, 5, 6, 7, 8, 9, ALL OFF
    byte tab[11] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0xff};
    // a, b, c, d, e, f, g, h, i, j, k, l, o, m, n, o, p, q, r, s, t, u, v, w, x, y, z
    byte taf[26] = {0xA0, 0x83, 0xa7, 0xa1, 0x86, 0x8e, 0xc2, 0x8b, 0xe6, 0xe1, 0x89, 0xc7, 0xaa, 0xc8, 0xa3, 0x8c,
                    0x98, 0xce, 0x9b, 0x87, 0xc1, 0xe3, 0xd5, 0xb6, 0x91, 0xb8};
    // "space", "."
    byte tap[2] = {0xff, 0x7f};
};


#endif //SIMCONSOLE_DFROBOT7SEG_H
