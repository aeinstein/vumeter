#include <Adafruit_NeoPixel.h>
#include "config.h"
#include "functions.h"


void setup() {
  Serial.begin(57600);           //  setup serial
  pinMode(LED_DATA_LEFT, OUTPUT);

  #ifdef STEREO
    #if MODE == DUAL
      pinMode(LED_DATA_RIGHT, OUTPUT);
    #endif
  #endif

  // For Debug only
  //dumpConfig();
  //saveSettings();


  if(hasStoredConfig()) {
  	Serial.println("load stored values");
  	loadSettings();

  } else {
    Serial.println("save initial config");
    saveSettings();
  }

  dumpConfig();
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
}

void loop() {
#ifdef CALIBRATION_POTI
  AMPLIFY = map(getAnalogIN(CALIBRATION_POTI), 0, 1024, 1, 3);
#endif

  leftVal = map(getAnalogIN(Left_IN) * AMPLIFY, 0, 1024, 0, NUM_LEDS);  // read the input pin

#ifdef STEREO
  rightVal = map(getAnalogIN(Right_IN) * AMPLIFY, 0, 1024, 0, NUM_LEDS);  // read the input pin
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
