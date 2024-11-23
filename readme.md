# VU-Meter
Fast and dirty VU-Meter with NeoPixel LedStrips and teensy lc for my home DJ Desk


## Features
* Mono/Stereo
* customizable number of leds
* 2 color effects
* Peak indicator
* build for [teensy lc](https://www.pjrc.com/teensy/teensyLC.html)
* compatibel with all arduino boards
* compatibel with all neopixel strips


## Example Setup
* [LedStrip](https://www.adafruit.com/product/1507)
* [teensy lc](https://www.pjrc.com/teensy/teensyLC.html)
* [Keystone Chassis](https://www.reichelt.de/index.html?ACTION=446&LA=3&nbc=1&q=delock%2086227)
* [PowerConnectors](https://www.reichelt.de/index.html?ACTION=446&LA=446&nbc=1&q=delock%2086303%20)
* [DataConnectors](https://www.reichelt.de/index.html?ACTION=446&LA=446&nbc=1&q=delock%2086323)
* [AnalogConnectors](https://www.reichelt.de/index.html?ACTION=446&LA=446&nbc=1&q=goobay%2079669%20)

Power supply 5V. max. current depends on number of leds.

## Compile
* Download [Arduino IDE](https://www.arduino.cc/en/software).
* Under Settings add [Teensy Lib](https://www.pjrc.com/teensy/package_teensy_index.json) as additional Board Url
* After download, choose teensy board in options
* Add [Adafruit_NeoPixel](https://github.com/adafruit/Adafruit_NeoPixel) as library


## Config Options
```
// Analog Inputs
#define Left_IN A1
#define Right_IN A2

// NeoPixel Datalines
#define LED_DATA_LEFT 1
#define LED_DATA_RIGHT 2

// Number of Leds per strip
#define NUM_LEDS 32

// Dimm to 10% because its super bright
#define BRIGHTNESS 0.1f

// idle white
#define GLOWNESS 0

// Use Stereo input. If Mono then only left channel is used
#define STEREO

// Number of analog probes in a row. Value is avarage.
#define NUMREADS 10

// Peek, default decay 0.03f
#define PEAK_INDICATOR
#define INDICATOR_STYLE 1
#define PEAK_DECAY 0.03f

// delay per loop, default 5ms
#define DELAY_MS 10
```

## Upcoming
* more Color effects
* more Led Layout types