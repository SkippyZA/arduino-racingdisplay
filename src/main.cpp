#ifndef UBRRH
#define UBRRH
#endif

#ifndef __AVR__
#define __AVR__
#endif

#define PIXEL_COUNT 16
#define SERIAL_BUFFER_LENGTH 20
#define GEAR_ADDRESS 0x4

//#define DEBUG

#include <Arduino.h>
#include <Wire.h>
#include <HardwareSerial.h>
#include <Adafruit_NeoPixel.h>
#include <Messenger.h>
#include "DFRobot7Seg.h"

Messenger message = Messenger('|');
char buffer[SERIAL_BUFFER_LENGTH];

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

uint8_t pixelDataPin = 11;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(16, pixelDataPin, NEO_GRB + NEO_KHZ800);
PixelColor pixelColors[PIXEL_COUNT];
uint8_t pixelMap[PIXEL_COUNT] = { 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
uint16_t lastPixelDisplay = 65535;

void updatePixels(uint16_t display) {
    lastPixelDisplay = display;

    for(uint8_t i = 0; i < PIXEL_COUNT; i++) {
        uint16_t toWrite = display & (1 << i);

        if(toWrite) {
            pixels.setPixelColor(i, pixelColors[i].r, pixelColors[i].g, pixelColors[i].b);
        } else {
            pixels.setPixelColor(i, 0, 0, 0);
        }
    }

    pixels.show();
}

void updateGear(int8_t gear) {

    switch (gear) {
        case 0:
            gear = 10;
            break;
        case -1:
            gear = 11;
            break;
        default:
            break;
    }

    Wire.beginTransmission(GEAR_ADDRESS); // transmit to device GEAR_ADDRESS
    Wire.write(gear);                     // sends one byte
    Wire.endTransmission();               // stop transmitting
}

uint8_t command = 0;
uint8_t pixelIndex;
PixelColor color;

void messageReady() {
    command = (uint8_t) message.readInt();
    message.copyString(buffer, SERIAL_BUFFER_LENGTH);

    switch(command) {
            case 1:
                pixels.setBrightness((uint8_t) atoi(buffer));
                pixels.show();
                break;
            case 2:
                pixelIndex = (uint8_t) atoi(strtok(buffer, "="));

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
                updatePixels((uint16_t) atoi(buffer));
                break;
            case 4:
                display.print(buffer);
                break;
            case 5:
                updateGear((int8_t) atoi(buffer));
            default:
                break;
    }
}

void setup() {
    Wire.begin();

    Serial.begin(9600);
    Serial.println("RacingLedDisplay");
    Serial.println("----------------");

    display.print("        ");
    pixels.begin();

    pixels.setBrightness(10);
    pixels.show();
    updatePixels(lastPixelDisplay);

    updateGear(0);

    message.attach(messageReady);
}

void loop() {

    while (Serial.available()) {
        message.process(Serial.read());
    }

    display.update();
}