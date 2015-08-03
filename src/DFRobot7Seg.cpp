//
// Created by Steven Inskip on 2015/06/24.
//

#include <HardwareSerial.h>
#include "DFRobot7Seg.h"

void DFRobot7Seg::print(const char *data) {
    const char *dataPtr;
    uint8_t count = 0;
    byte byteToSet;
    uint8_t bitToSet;

    for(dataPtr = data; *dataPtr != '\0' && *dataPtr != '\r'; dataPtr++) {
        if (*dataPtr == 32) {                   // test for "space"
            byteToSet = tap[0];
        }
        else if (*dataPtr == 46) {              // test for "."
            byteToSet = tap[1];
        }
        else if (*dataPtr <= 57) {              // test for numbers
            bitToSet = (uint8_t) (*dataPtr - 48);
            byteToSet = tab[bitToSet];
        }
        else {                                  // test for letters
            bitToSet = (uint8_t) (*dataPtr - 97);
            byteToSet = taf[bitToSet];
        }

        buffer[count++] = byteToSet;
    }

//    for(dataPtr = data; *dataPtr != '\0' && *dataPtr != '\r'; dataPtr++) {
//        digitalWrite(latchPin, LOW);
//
//        if (*dataPtr == 32) {                   // test for "space"
//            shiftOut(dataPin, clockPin, MSBFIRST, tap[0]);
//        }
//        else if (*dataPtr == 46) {              // test for "."
//            shiftOut(dataPin, clockPin, MSBFIRST, tap[1]);
//        }
//        else if (*dataPtr <= 57) {              // test for numbers
//            bitToSet = (uint8_t) (*dataPtr - 48);
//            shiftOut(dataPin, clockPin, MSBFIRST, tab[bitToSet]);
//        }
//        else {                                  // test for letters
//            bitToSet = (uint8_t) (*dataPtr - 97);
//            shiftOut(dataPin, clockPin, MSBFIRST, taf[bitToSet]);
//        }
//
//        digitalWrite(latchPin, HIGH);
//
//    }
}

void DFRobot7Seg::print(String data) {
    data.toLowerCase();
    print(data.c_str());
}

void DFRobot7Seg::update() {
    digitalWrite(latchPin, LOW);

    for(uint8_t i = 0; i < SEGMENT_COUNT; i++) {
        uint8_t toWrite = (~buffer[i]) & (0b10000000 >> bitIndex);
        toWrite = ~toWrite; // Flip it for common cathode

        if (!toWrite) {
            return;
        }

        shiftOut(dataPin, clockPin, MSBFIRST, toWrite);
    }

    digitalWrite(latchPin, HIGH);

    if(++bitIndex >= 8) {
        bitIndex = 0;
    }
}
