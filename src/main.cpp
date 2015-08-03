#ifndef UBRRH
#define UBRRH
#endif

#ifndef __AVR__
#define __AVR__
#endif

#define PIXEL_COUNT 16
#define SERIAL_BUFFER_LENGTH 512

#include <Arduino.h>
#include <HardwareSerial.h>
#include <Adafruit_NeoPixel.h>
#include "DFRobot7Seg.h"

// 8x 7seg
//////////////////////////////////////////////////////
uint8_t clockDisplayPin = 3;
uint8_t latchDisplayPin = 8;
uint8_t dataDisplayPin = 9;
DFRobot7Seg display(clockDisplayPin, latchDisplayPin, dataDisplayPin);

// Pixel Crap
//////////////////////////////////////////////////////
struct PixelColor {
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
};

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(16, 11, NEO_GRB + NEO_KHZ800);
PixelColor pixelColors[PIXEL_COUNT];
uint8_t pixelMap[PIXEL_COUNT] = { 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
uint16_t lastPixelDisplay = 0;

// This is for the serial crap
//////////////////////////////////////////////////////
struct SerialMessage {
    uint8_t command;
    char* value;
};

uint16_t serialByteCounter = 0;
char inputString[SERIAL_BUFFER_LENGTH];
SerialMessage incomingSerialMessage;
boolean stringComplete = false;

void updatePixels(uint16_t display) {
    lastPixelDisplay = display;

    for(uint8_t i = 0; i < PIXEL_COUNT; i++) {
        uint16_t toWrite = display & (0b0000000000000001 << i);

        if(toWrite) {
            pixels.setPixelColor(i, pixelColors[i].r, pixelColors[i].g, pixelColors[i].b);
        } else {
            pixels.setPixelColor(i, 0, 0, 0);
        }
    }

    pixels.show();
}

void setup() {
    Serial.begin(9600);
    Serial.println("RacingLedDisplay");
    Serial.println("----------------");

    display.print("        ");
    pixels.begin();

    pixels.setBrightness(10);
    updatePixels(lastPixelDisplay);
}

void loop() {
    uint8_t pixelIndex;
    PixelColor color;

    if (stringComplete) {
        Serial.print("Command: ");
        Serial.print(incomingSerialMessage.command);
        Serial.print(", Value: ");
        Serial.println(incomingSerialMessage.value);

        switch(incomingSerialMessage.command) {
            case 1:
                pixels.setBrightness((uint8_t) atoi(incomingSerialMessage.value));
                pixels.show();
                break;
            case 2:
                pixelIndex = (uint8_t) atoi(strtok(incomingSerialMessage.value, "="));

                color.r = (uint8_t) atoi(strtok(NULL, ","));
                color.g = (uint8_t) atoi(strtok(NULL, ","));
                color.b = (uint8_t) atoi(strtok(NULL, ","));

#ifdef DEBUG
                Serial.print("Pixel: ");
                Serial.print(pixelIndex);
                Serial.print(", Color: ");
                Serial.print(color.r);
                Serial.print(", ");
                Serial.print(color.g);
                Serial.print(", ");
                Serial.println(color.b);
#endif

                pixelColors[pixelMap[pixelIndex]] = color;
                break;
            case 3:
                updatePixels((uint16_t) atoi(incomingSerialMessage.value));
                break;
            case 4:
                display.print(incomingSerialMessage.value);
                break;
            default:
                break;
        }

        stringComplete = false;
    }
}

void serialEvent() {
    while (Serial.available()) {
        char inChar = (char) Serial.read();

        if(inChar == '|') {
            inputString[serialByteCounter] = '\0';
            incomingSerialMessage.command = (uint8_t) atoi(inputString);
            serialByteCounter = 0;
        } else if (inChar == '\n') {
            inputString[serialByteCounter++] = '\0';
        } else {
            inputString[serialByteCounter++]= inChar;
        }

        if (inChar == '\n') {
            incomingSerialMessage.value = inputString;

            stringComplete = true;
            serialByteCounter = 0;
        }
    }
}