# Arduino Racing Display
This is a little hobbie electronics project I threw together and forms part of the Java SimConsole and GearIndicator on my GitHub page.

Using the [DFRobot 8 Character 7 Segment SPI Module](http://www.robotshop.com/en/dfrobot-8-character-7-segment-spi-led-module.html) and 2x [NeoPixel Stick - 8x RGB LED](https://www.adafruit.com/products/1426) and an Arduino Uno, this device acts as a HUD for displaying the current car revs, speed, etc. To reduce current required, the 8 character 7 segement is multiplex to only allow 1 LED per character to display at once.

## Protocol

### Set pixel brightness
- `1|<u_int8>`

eg: `1|10` sets brightness to 10

### Set pixel colour by index 
- `2|<u_int8>=<u_int8>,<u_int8>,<u_int8>`

eg: `2|4=255,0,0` sets pixel at index 2 to the colour red

### Display pixels based on an integer
- `3|<u_int16>`

eg: `3|49155`  which will display the 2 outside pixels on either side as `49155 = 1100000000000011`

### Display text on segment display
- `4|<char[8]>`

eg: `4|hellowor` will display `hellowor` on the LED display

### Update gear indicator
- `5|<u_int8>`

eg: `5|3` will set the gear indicator to display 3

---

Prototyping some ideas

![prototying some ideas](http://i.imgur.com/GJJd8CB.jpg?1)

First iteration

![first iteration](http://i.imgur.com/SyNfM5q.jpg?1)

Updated for a better location

![updated for a better location](http://i.imgur.com/09S1FXD.jpg?1)

