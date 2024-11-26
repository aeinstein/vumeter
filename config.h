#ifndef CONFIG
#define CONFIG

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

// Number of leds per channel
unsigned int NUM_LEDS = 14;

// 10%
float BRIGHTNESS = 0.1f;

// idle color
unsigned int GLOWNESS = 0;

// decay of channel value
float CHANNEL_DECAY = 0.5f;

// Peak, default decay 0.05f
float PEAK_DECAY = 0.1f;

// input calibration
float AMPLIFY = 2.5f;

// The Colors
int lowColor[] = {   0, 255,   0};
int midColor[] = {   0,   0, 255};
int highColor[] ={ 255,   0,   0};

#endif
