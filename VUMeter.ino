#include <Adafruit_NeoPixel.h>
#include "config.h"
#include "functions.h"


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

#ifdef PEAK_INDICATOR
  Serial.println("Peak Indicator");
#endif
}

void loop() {
  leftVal = getAnalogIN(Left_IN) * NUM_LEDS / 1024;  // read the input pin

#ifdef STEREO
  rightVal = getAnalogIN(Right_IN) * NUM_LEDS / 1024;  // read the input pin
#endif

  if(leftVal > leftCurrent) leftCurrent = leftVal;
  if(rightVal > rightCurrent) rightCurrent = rightVal;

#ifdef PEAK_INDICATOR
	if(leftVal > leftPeak) leftPeak = leftVal + 1.5f;   // +1 so indicator stays above level and +0.5 prevent flickering
	if(rightVal > rightPeak) rightPeak = rightVal + 1.5f;
#endif

  setLeds(leftCurrent, rightCurrent);
  delay(DELAY_MS);

  // decay
  if(leftCurrent > 0) leftCurrent--;
  if(rightCurrent > 0) rightCurrent--;

#ifdef PEAK_INDICATOR
  if(leftPeak > 0) leftPeak -= PEAK_DECAY;
  if(rightPeak > 0) rightPeak -= PEAK_DECAY;
#endif
}
