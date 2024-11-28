#include <Adafruit_NeoPixel.h>
#include "config.h"
#include "functions.h"
#include "console.h"

void setup() {
  Serial.begin(57600);           //  setup serial
  pinMode(LED_DATA_LEFT, OUTPUT);

  #ifdef STEREO
    #if MODE == DUAL
      pinMode(LED_DATA_RIGHT, OUTPUT);
    #endif
  #endif

  // If no config, write one
  if(!loadSettings()) saveSettings();

  dumpConfig();

  cli_init();
}


void loop() {
#ifdef CALIBRATION_POTI
  AMPLIFY = map(getAnalogIN(CALIBRATION_POTI), 0, 1024, 1, 3);
#endif

  getAnalogIN(&leftVal, &rightVal);

  leftVal = map(leftVal * AMPLIFY, 0, 1024, 0, NUM_LEDS);  // read the input pin
  leftVal = compress(leftVal, NUM_LEDS, 3);

#ifdef STEREO
  rightVal = map(rightVal * AMPLIFY, 0, 1024, 0, NUM_LEDS);  // read the input pin
  rightVal = compress(rightVal, NUM_LEDS, 3);
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


  my_cli();
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
