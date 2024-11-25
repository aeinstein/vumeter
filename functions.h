#include "config.h"

int leftVal = 0;
int rightVal = 0;

float leftCurrent = 0;
float rightCurrent = 0;

float leftPeak = 0;
float rightPeak = 0;

#if MODE == DUAL
  Adafruit_NeoPixel pixels_left = Adafruit_NeoPixel(NUM_LEDS, LED_DATA_LEFT, NEO_GRB + NEO_KHZ800);

  #ifdef STEREO
    Adafruit_NeoPixel pixels_right = Adafruit_NeoPixel(NUM_LEDS, LED_DATA_RIGHT, NEO_GRB + NEO_KHZ800);
  #endif

#else
  #ifdef STEREO
    Adafruit_NeoPixel pixels_left = Adafruit_NeoPixel(NUM_LEDS *2, LED_DATA_LEFT, NEO_GRB + NEO_KHZ800);
  #endif

  #ifndef STEREO
    Adafruit_NeoPixel pixels_left = Adafruit_NeoPixel(NUM_LEDS, LED_DATA_LEFT, NEO_GRB + NEO_KHZ800);
  #endif
#endif

int* getColor(int val, int i);
int* getColor2(int val, int i);

int translateRightChannel(int val){
  #if MODE == DUAL  
    return val;

  #elif MODE == STRIPPED
    return val + NUM_LEDS;

  #elif MODE == MIRROR
    return val + NUM_LEDS;

  #else
    return NUM_LEDS *2 - val -1;
  #endif
}

int translateLeftChannel(int val){
  #if MODE == MIRROR
    return NUM_LEDS - val -1;
  #else
    return val;
  #endif
}

int getAnalogIN(int pin){
  int tmp = 0;

  for(int i = 0; i < NUMREADS; i++) {
    int curr = analogRead(pin) * AMPLIFY;
    if(curr > tmp) tmp = curr;
  }

  if(tmp > 1024) tmp = 1024;    // Fix Maxvalue

  return tmp;
}

void setLeds(int leftVal, int rightVal){
  int *color;

  for(int i = 0; i < NUM_LEDS; i++){
      color = getColor2(leftVal, i);

      pixels_left.setPixelColor(translateLeftChannel(i), pixels_left.Color(color[0] * BRIGHTNESS, color[1] * BRIGHTNESS, color[2] * BRIGHTNESS));

      delete[] color;

      #ifdef STEREO
        color = getColor2(rightVal, i);

        #if MODE == DUAL  
          pixels_right.setPixelColor(translateRightChannel(i), pixels_right.Color(color[0] * BRIGHTNESS, color[1] * BRIGHTNESS, color[2] * BRIGHTNESS));

        #else
          pixels_left.setPixelColor(translateRightChannel(i), pixels_left.Color(color[0] * BRIGHTNESS, color[1] * BRIGHTNESS, color[2] * BRIGHTNESS));
        #endif

        delete[] color;

      #endif
  }

  #ifdef PEAK_INDICATOR
  	color = getColor((int)leftPeak, (int)leftPeak);

    #if MODE == MIRROR
  	  pixels_left.setPixelColor(NUM_LEDS - (int)leftPeak, pixels_left.Color(color[0] * BRIGHTNESS, color[1] * BRIGHTNESS, color[2] * BRIGHTNESS));
    #else
      pixels_left.setPixelColor((int)leftPeak, pixels_left.Color(color[0] * BRIGHTNESS, color[1] * BRIGHTNESS, color[2] * BRIGHTNESS));
    #endif

    delete[] color;
  #endif

  #ifdef STEREO
    #ifdef PEAK_INDICATOR
      color = getColor((int)rightPeak, (int)rightPeak);

      #if MODE == DUAL  
        pixels_right.setPixelColor(translateRightChannel((int)rightPeak), pixels_right.Color(color[0] * BRIGHTNESS, color[1] * BRIGHTNESS, color[2] * BRIGHTNESS));
      #else
        pixels_left.setPixelColor(translateRightChannel((int)rightPeak), pixels_left.Color(color[0] * BRIGHTNESS, color[1] * BRIGHTNESS, color[2] * BRIGHTNESS));
      #endif

      delete[] color;
    #endif
  #endif

  pixels_left.show();

  #if MODE == DUAL
    pixels_right.show();
  #endif
}

void copyColor(int* src, int* dst) {
    for (int i = 0; i < 3; i++) *dst++ = *src++;
}

// Led Colors remain
int* getColor2(int val, int i){
  int *tmpColor = new int[3];

  if(val >= i) {
    if(i > 0.9 * NUM_LEDS) {
      copyColor(highColor, tmpColor);

    } else if(i >= 0.5 * NUM_LEDS) {
      copyColor(midColor, tmpColor);

    } else {
      copyColor(lowColor, tmpColor);
    }

  } else {
    tmpColor[0] = GLOWNESS;
    tmpColor[1] = GLOWNESS;
    tmpColor[2] = GLOWNESS;
  }

  return tmpColor;
}

// led colors change to level color
int* getColor(int val, int i){
  int *tmpColor = new int[3];

  if(val >= i) {
    if(val > 0.9 * NUM_LEDS) {
      copyColor(highColor, tmpColor);

    } else if(val >= 0.5 * NUM_LEDS) {
      copyColor(midColor, tmpColor);

    } else {
      copyColor(lowColor, tmpColor);
    }

  } else {
    tmpColor[0] = GLOWNESS;
    tmpColor[1] = GLOWNESS;
    tmpColor[2] = GLOWNESS;
  }

  return tmpColor;
}
