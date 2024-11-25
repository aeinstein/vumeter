#ifndef CONFIG
#define CONFIG

// Analog in pins
#define Left_IN A1
#define Right_IN A2

// Data out pins
#define LED_DATA_LEFT 2
#define LED_DATA_RIGHT 1

// Number of leds per channel
#define NUM_LEDS 28

// 10%
#define BRIGHTNESS 0.1f

// idle color
#define GLOWNESS 0

// Use Stereo input. If mono then only left channel is used
//#define STEREO

// Number of analog probes
#define NUMREADS 10

// momentum of channel value
#define CHANNEL_DECAY 0.5f

// Peak, default decay 0.05f
#define PEAK_INDICATOR
#define INDICATOR_STYLE 1
#define PEAK_DECAY 0.1f

// delay per loop, default 5ms
#define DELAY_MS 20

#define AMPLIFY 2.5f


// led stripe modes
#define DUAL 1
#define STRIPPED 2
#define MIRROR 3
#define FOLDED 4

#define MODE STRIPPED

int lowColor[] = {   0, 255,   0};
int midColor[] = {   0,   0, 255};
int highColor[] ={ 255,   0,   0};

#endif

