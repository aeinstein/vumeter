#include "config.h"
#include "store.h"

int leftVal = 0;
int rightVal = 0;

float leftCurrent = 0;
float rightCurrent = 0;

float leftPeak = 0;
float rightPeak = 0;

float amplifyFactor;


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

unsigned long getColor(int val, int i);
unsigned long getColor2(int val, int i);

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

void getAnalogIN(int *left, int *right){
  *left = 0;
  *right = 0;

  int curr = 0;

  for(int i = 0; i < NUMREADS; i++) {
    curr = analogRead(Left_IN);
    if(curr > *left) *left = curr;

    #ifdef STEREO
      curr = analogRead(Right_IN);
      if(curr > *right) *right = curr;
    #endif

    delayMicroseconds(100); // Max freq is 10kHz    1/10000 = 0.1ms. 20 reads * 0.1ms = 2ms = 500hz 
    // so freq is 500hz - 10khz
    // the very pour fourier transformation ;-)
  }
}

void setLeds(int leftVal, int rightVal){
  unsigned long color;
  int r,g,b;

  for(unsigned int i = 0; i < NUM_LEDS; i++){
      color = getColor2(leftVal, i);

      r = (color >> 16) & 0xff;
      g = (color >> 8) & 0xff;
      b = (color) & 0xff;

      pixels_left.setPixelColor(translateLeftChannel(i), pixels_left.Color(r * BRIGHTNESS, g * BRIGHTNESS, b * BRIGHTNESS));

      #ifdef STEREO
        color = getColor2(rightVal, i);

        r = (color >> 16) & 0xff;
        g = (color >> 8) & 0xff;
        b = (color) & 0xff;

        #if MODE == DUAL  
          pixels_right.setPixelColor(translateRightChannel(i), pixels_right.Color(r * BRIGHTNESS, g * BRIGHTNESS, b * BRIGHTNESS));

        #else
          pixels_left.setPixelColor(translateRightChannel(i), pixels_left.Color(r * BRIGHTNESS, g * BRIGHTNESS, b * BRIGHTNESS));
        #endif
      #endif
  }

  #ifdef PEAK_INDICATOR
  	color = getColor((int)leftPeak, (int)leftPeak);

    r = (color >> 16) & 0xff;
    g = (color >> 8) & 0xff;
    b = (color) & 0xff;

    #if MODE == MIRROR
  	  pixels_left.setPixelColor(NUM_LEDS - (int)leftPeak, pixels_left.Color(r * BRIGHTNESS, g * BRIGHTNESS, b * BRIGHTNESS));
    #else
      pixels_left.setPixelColor((int)leftPeak, pixels_left.Color(r * BRIGHTNESS, g * BRIGHTNESS, b * BRIGHTNESS));
    #endif
  #endif

  #ifdef STEREO
    #ifdef PEAK_INDICATOR
      color = getColor((int)rightPeak, (int)rightPeak);

      r = (color >> 16) & 0xff;
      g = (color >> 8) & 0xff;
      b = (color) & 0xff;

      #if MODE == DUAL  
        pixels_right.setPixelColor(translateRightChannel((int)rightPeak), pixels_right.Color(r * BRIGHTNESS, g * BRIGHTNESS, b * BRIGHTNESS));
      #else
        pixels_left.setPixelColor(translateRightChannel((int)rightPeak), pixels_left.Color(r * BRIGHTNESS, g * BRIGHTNESS, b * BRIGHTNESS));
      #endif
    #endif
  #endif

  pixels_left.show();

  #if MODE == DUAL
    pixels_right.show();
  #endif
}

/*
void copyColor(int* src, int* dst) {
    for (int i = 0; i < 3; i++) *dst++ = *src++;
}*/

// Led Colors remain
unsigned long getColor2(int val, int i){
  if(val >= i) {
    if(i > 0.9 * NUM_LEDS) {
      return HC;

    } else if(i >= 0.5 * NUM_LEDS) {
      return MC;

    } else {
      return LC;
    }
  } 
  return GLOWNESS;
}

// led colors change to level color
unsigned long getColor(int val, int i){
  if(val >= i) {
    if(val > 0.9 * NUM_LEDS) {
      return HC;

    } else if(val >= 0.5 * NUM_LEDS) {
      return MC;

    } else {
      return LC;
    }
  } 

  return GLOWNESS;
}

void dumpConfig(){
  Serial.println("current Config");
  Serial.print("AMPLIFY: ");
  Serial.println(AMPLIFY, 5);

  Serial.print("CHANNEL_DECAY: ");
  Serial.println(CHANNEL_DECAY, 5);

  #ifdef PEAK_INDICATOR
    Serial.println("Peak Indicator");
    Serial.print("PEAK_DECAY: ");
    Serial.println(PEAK_DECAY, 5);
  #endif

  #ifdef STEREO
    Serial.println("Stereo");
  #endif

  #ifndef STEREO
    Serial.println("Mono");
  #endif

  #if MODE == DUAL
    Serial.println("2 Data out channels");
    Serial.println("Dual mode");

  #else
    Serial.println("1 Data out channel");

    #if MODE == STRIPPED
      Serial.println("Stripped mode");
    #endif

    #if MODE == MIRROR
      Serial.println("Mirror mode");
    #endif

    #if MODE == FOLDED
      Serial.println("Folded mode");
    #endif
  #endif

  Serial.print("Number of leds: ");
  Serial.println(NUM_LEDS, DEC);

  Serial.print("BRIGHTNESS: ");
  Serial.println(BRIGHTNESS, 5);

  Serial.print("GLOWNESS: ");
  Serial.println(GLOWNESS, 5);

  Serial.println("led mapping");

  for(unsigned int i = 0; i < NUM_LEDS; i++){
    Serial.print(i, DEC);
    Serial.print(" -> left ");
    Serial.print(translateLeftChannel(i), DEC); 

    #ifdef STEREO
      Serial.print(" -> right ");
      Serial.print(translateRightChannel(i), DEC);
    #endif

    Serial.println("");
  }

  Serial.println("Colors:");
  Serial.print("Low Color: ");
  Serial.println(LC, HEX);

  Serial.print("Mid Color: ");
  Serial.println(MC, HEX);

  Serial.print("High Color: ");
  Serial.println(HC, HEX);
}
