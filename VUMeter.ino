#include <Adafruit_NeoPixel.h>
#include "config.h"
#include "functions.h"


void setup() {
  Serial.begin(57600);           //  setup serial
  pinMode(LED_DATA_LEFT, OUTPUT);

#ifdef STEREO
  Serial.println("Stereo Mode");

  #if MODE == DUAL
    pinMode(LED_DATA_RIGHT, OUTPUT);
    Serial.println("2 Data out channels");
  #else
    Serial.println("1 Data out channel");
  #endif
#endif

#ifndef STEREO
  Serial.println("Mono Mode");
#endif

#ifdef PEAK_INDICATOR
  Serial.println("Peak Indicator");
#endif


  Serial.println("led mapping");

  for(int i = 0; i < NUM_LEDS; i++){
    Serial.print(i, DEC);
    Serial.print(" -> left ");
    Serial.print(translateLeftChannel(i), DEC); 
    Serial.print(" -> right ");
    Serial.print(translateRightChannel(i), DEC);
    Serial.println("");
  }
}

void loop() {
  leftVal = getAnalogIN(Left_IN) * NUM_LEDS / 1024;  // read the input pin

#ifdef STEREO
  rightVal = getAnalogIN(Right_IN) * NUM_LEDS / 1024;  // read the input pin
#endif

  if(leftVal > leftCurrent) leftCurrent = leftVal;
  if(rightVal > rightCurrent) rightCurrent = rightVal;

#ifdef PEAK_INDICATOR
	if(leftVal >= leftPeak) leftPeak = leftVal + 0.5f;   // +1 so indicator stays above level and +0.5 prevent flickering
	if(rightVal >= rightPeak) rightPeak = rightVal + 0.5f;

  // ensure to not exceed channel
  if(leftPeak > NUM_LEDS -1) leftPeak = NUM_LEDS -1;
  if(rightPeak > NUM_LEDS -1) rightPeak = NUM_LEDS -1;
#endif

  setLeds(leftCurrent, rightCurrent);
  delay(DELAY_MS);

  // decay
  if(leftCurrent > 0) leftCurrent -= CHANNEL_DECAY;
  if(rightCurrent > 0) rightCurrent -= CHANNEL_DECAY;

#ifdef PEAK_INDICATOR
  if(leftVal + 0.5f < leftPeak && leftPeak > 0) leftPeak -= PEAK_DECAY;
  if(rightVal + 0.5f < rightPeak && rightPeak > 0) rightPeak -= PEAK_DECAY;
#endif
}
