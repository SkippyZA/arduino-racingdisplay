cmake_minimum_required(VERSION 3.2)
project(SimConsole)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    src/main.cpp)

include_directories("lib/Adafruit_NeoPixel")
include_directories("lib/Wire")

include_directories("/Users/steven/.platformio/packages/framework-arduinoavr/cores/arduino/")
include_directories("/Users/steven/.platformio/packages/toolchain-atmelavr/avr/include")

add_executable(SimConsole ${SOURCE_FILES} src/DFRobot7Seg.cpp src/DFRobot7Seg.h)