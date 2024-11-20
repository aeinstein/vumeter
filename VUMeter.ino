#include <Adafruit_NeoPixel.h>

#define Left_IN A1
#define Right_IN A2

#define LED_DATA_LEFT 1
#define LED_DATA_RIGHT 2

#define NUM_LEDS 32

// 10% 
#define BRIGHTNESS 0.1f

// 
#define GLOWNESS 0

// Use Stereo input. If Mono then only left channel is used
#define STEREO

// Number of analog probes
#define NUMREADS 10


int leftVal = 0;
int rightVal = 0;

int leftCurrent = 0;
int rightCurrent = 0;

Adafruit_NeoPixel pixels_left = Adafruit_NeoPixel(NUM_LEDS, LED_DATA_LEFT, NEO_GRB + NEO_KHZ800);

#ifdef STEREO
Adafruit_NeoPixel pixels_right = Adafruit_NeoPixel(NUM_LEDS, LED_DATA_RIGHT, NEO_GRB + NEO_KHZ800);
#endif



void setup() {
  Serial.begin(57600);           //  setup serial
  pinMode(LED_DATA_LEFT, OUTPUT);

#ifdef STEREO
  pinMode(LED_DATA_RIGHT, OUTPUT);
  Serial.println("Stereo Mode");
#endif

#ifndef STEREO
  Serial.println("Mono Mode");
#endif
}

void loop() {
  leftVal = getAnalogIN(Left_IN) * NUM_LEDS / 1024;  // read the input pin

#ifdef STEREO
  rightVal = getAnalogIN(Right_IN) * NUM_LEDS / 1024;  // read the input pin
#endif

  if(leftVal > leftCurrent) leftCurrent = leftVal;
  if(rightVal > rightCurrent) rightCurrent = rightVal;

  setLeds(leftCurrent, rightCurrent);

  /*
  int* color = getColor(75,40);
  char buffer [50];
  sprintf (buffer, "color: %d %d %d", color[0], color[1], color[2]);
  Serial.println(buffer);
  */
  //Serial.println("delay");
  
  delay(5);

  // decay
  if(leftCurrent > 0) leftCurrent--;
  if(rightCurrent > 0) rightCurrent--;
}

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
  for(int i = 0; i < NUM_LEDS; i++){
      int *color = getColor(leftVal, i);
      pixels_left.setPixelColor(i, pixels_left.Color(color[0] * BRIGHTNESS, color[1] * BRIGHTNESS, color[2] * BRIGHTNESS));
      delete[] color;
    
      #ifdef STEREO
        int *color2 = getColor2(rightVal, i);
        pixels_right.setPixelColor(i, pixels_left.Color(color2[0] * BRIGHTNESS, color2[1] * BRIGHTNESS, color2[2] * BRIGHTNESS));
        delete[] color2;
      #endif
  }

  pixels_left.show();

  #ifdef STEREO
    pixels_right.show();
  #endif
}

// Led Colors remain
int* getColor2(int val, int i){
  int *tmpColor = new int[3];

  if(val > i) {
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

  if(val > i) {
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


