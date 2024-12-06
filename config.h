#ifndef CONFIG
#define CONFIG

// Hardware Config
// Use Stereo input. If mono then only left channel is used
#define STEREO

// Analog in pins
#define Left_IN A2
#define Right_IN A1

// Data out pins
#define LED_DATA_LEFT 2
#define LED_DATA_RIGHT 1

// led stripe modes
#define DUAL 1
#define STRIPPED 2
#define MIRROR 3
#define FOLDED 4

// Install poti, for onthefly calibration
#define CALIBRATION_POTI A3

// delay per loop, default 5ms
#define DELAY_MS 5

// Number of analog probes
#define NUMREADS 20

// OLED Display
#define OLED

// Peak indicator
bool PEAK_INDICATOR = true;

// Default Config Values on first Start
// These are editable on the fly

// led stripe mode
byte MODE = STRIPPED;

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

#endif
