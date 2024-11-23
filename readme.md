# VU-Meter
Fast and dirty VU-Meter with NeoPixel LedStrips and teensy lc for my home DJ Desk


## Features
* Mono/Stereo
* customizable number of leds
* 2 color effects
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
```

## Upcoming
* more Color effects
* more Led Layout types