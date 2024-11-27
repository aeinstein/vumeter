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
* Config saved in eeprom
* Serial Console for on the fly changes


## Example Setup with Keystone Modules
* [LedStrip](https://www.adafruit.com/product/1507)
* [teensy lc](https://www.pjrc.com/teensy/teensyLC.html)
* [Keystone Chassis](https://www.reichelt.de/index.html?ACTION=446&LA=3&nbc=1&q=delock%2086227)
* [PowerConnectors](https://www.reichelt.de/index.html?ACTION=446&LA=446&nbc=1&q=delock%2086303%20)
* [DataConnectors](https://www.reichelt.de/index.html?ACTION=446&LA=446&nbc=1&q=delock%2086323)
* [AnalogConnectors](https://www.reichelt.de/index.html?ACTION=446&LA=446&nbc=1&q=goobay%2079669%20)

Power supply 5V. max. current depends on number of leds.


![Final](/images/keystone_setup.jpeg)

## Final build
![Final](/images/complete.jpeg)



## Compile
* Download [Arduino IDE](https://www.arduino.cc/en/software).
* Under Settings add [Teensy Lib](https://www.pjrc.com/teensy/package_teensy_index.json) as additional Board Url
* After download, choose teensy board in options
* Add [Adafruit_NeoPixel](https://github.com/adafruit/Adafruit_NeoPixel) as library

## Firstlight
![Final](/images/firstlight.jpeg)

## led stripe modes
### Two Strips
```
Dual Mode
 6    6
 5    5
 4    4
 3    3
 2    2
 1    1
 
D1   D2
```
### One Strip, seperated
```
Stripped   Mirror  Folded
   6          6       1
   5          5       2
   4          4       3
   3          3       4
   2          2       5
   1          1       6

   6          1       6
   5          2       5
   4          3       4
   3          4       3
   2          5       2
   1          6       1

  D1         D1      D1
```

## Config Options
```
// Hardware Config
// Use Stereo input. If mono then only left channel is used
#define STEREO

// Analog in pins
#define Left_IN A1
#define Right_IN A2

// Data out pins
#define LED_DATA_LEFT 2
#define LED_DATA_RIGHT 1

// led stripe modes
#define DUAL 1
#define STRIPPED 2
#define MIRROR 3
#define FOLDED 4

#define MODE STRIPPED

// Install poti, for onthefly calibration
//#define CALIBRATION_POTI A3

// delay per loop, default 5ms
#define DELAY_MS 20

// Number of analog probes
#define NUMREADS 20

// Peak indicator
#define PEAK_INDICATOR

// Default Config Values on first Start
// These are editable on the fly

// Number of leds per channel
unsigned int NUM_LEDS = 14;

// 10%
float BRIGHTNESS = 0.1f;

// idle color
//unsigned int GLOWNESS = 0;
unsigned long GLOWNESS = 0x000000;

// decay of channel value
float CHANNEL_DECAY = 0.5f;

// Peak, default decay 0.05f
float PEAK_DECAY = 0.1f;

// input calibration
float AMPLIFY = 2.5f;

// The Colors
unsigned long LC = 0x00FF00;
unsigned long MC = 0x0000FF;
unsigned long HC = 0xFF0000;
```

## Upcoming
* more Color effects
* trimmer for calibration
* OLED für Configuration
