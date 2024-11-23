#include "config.h"

int leftVal = 0;
int rightVal = 0;

int leftCurrent = 0;
int rightCurrent = 0;

float leftPeak = 0;
float rightPeak = 0;

Adafruit_NeoPixel pixels_left = Adafruit_NeoPixel(NUM_LEDS, LED_DATA_LEFT, NEO_GRB + NEO_KHZ800);

#ifdef STEREO
Adafruit_NeoPixel pixels_right = Adafruit_NeoPixel(NUM_LEDS, LED_DATA_RIGHT, NEO_GRB + NEO_KHZ800);
#endif

int* getColor(int val, int i);
int* getColor2(int val, int i);


int getAnalogIN(int pin){
  int tmp = 0;
  int cnt = 0;

  for(int i = 0; i < NUMREADS; i++) {
    int curr = analogRead(pin);

    // Count only positive values, because music is AC
    // and calc the middle
    if(curr >0) {
      tmp += curr;
      cnt++;
    }
  }

  return tmp / cnt;
}

void setLeds(int leftVal, int rightVal){
  int *color;

  for(int i = 0; i < NUM_LEDS; i++){
      color = getColor(leftVal, i);
      pixels_left.setPixelColor(i, pixels_left.Color(color[0] * BRIGHTNESS, color[1] * BRIGHTNESS, color[2] * BRIGHTNESS));
      delete[] color;

      #ifdef STEREO
        color = getColor2(rightVal, i);
        pixels_right.setPixelColor(i, pixels_left.Color(color[0] * BRIGHTNESS, color[1] * BRIGHTNESS, color[2] * BRIGHTNESS));
        delete[] color;
      #endif
  }

  #ifdef PEAK_INDICATOR
  	color = getColor((int)leftPeak, (int)leftPeak);
  	pixels_left.setPixelColor((int)leftPeak, pixels_left.Color(color[0] * BRIGHTNESS, color[1] * BRIGHTNESS, color[2] * BRIGHTNESS));
    delete[] color;
  #endif

  pixels_left.show();

  #ifdef STEREO
    #ifdef PEAK_INDICATOR
      color = getColor((int)rightPeak, (int)rightPeak);
      pixels_right.setPixelColor((int)rightPeak, pixels_right.Color(color[0] * BRIGHTNESS, color[1] * BRIGHTNESS, color[2] * BRIGHTNESS));
      delete[] color;
    #endif
    pixels_right.show();
  #endif
}

// Led Colors remain
int* getColor2(int val, int i){
  int *tmpColor = new int[3];

  if(val >= i) {
    if(i > 0.7 * NUM_LEDS) {
      tmpColor[0] = 255;
      tmpColor[1] = 0;
      tmpColor[2] = 0;

    } else if(i > 0.5 * NUM_LEDS) {
      tmpColor[0] = 255;
      tmpColor[1] = 255;
      tmpColor[2] = 0;

    } else {
      tmpColor[0] = 0;
      tmpColor[1] = 255;
      tmpColor[2] = 0;
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
    if(val > 0.7 * NUM_LEDS) {
      tmpColor[0] = 255;
      tmpColor[1] = 0;
      tmpColor[2] = 0;

    } else if(val > 0.5 * NUM_LEDS) {
      tmpColor[0] = 255;
      tmpColor[1] = 255;
      tmpColor[2] = 0;

    } else {
      tmpColor[0] = 0;
      tmpColor[1] = 255;
      tmpColor[2] = 0;
    }

  } else {
    tmpColor[0] = GLOWNESS;
    tmpColor[1] = GLOWNESS;
    tmpColor[2] = GLOWNESS;
  }

  return tmpColor;
}