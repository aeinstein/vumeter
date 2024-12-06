#include "config.h"
#include "functions.h"
#include "console.h"

#ifdef OLED
  #include "display.h"
  #include "menu.h"
#endif

void setup() {
  Serial.begin(115200);           //  setup serial
  pinMode(LED_DATA_LEFT, OUTPUT);

  // If no config, write one
  if(!loadSettings()) saveSettings();

  dumpConfig();
  initLeds();
  cli_init();

  #ifdef OLED
    display_setup();
  #endif
}


void loop() {
#ifdef CALIBRATION_POTI
  float tmpAmp = analogRead(CALIBRATION_POTI) / 400.0f;

  if(AMPLIFY > tmpAmp + 0.1 
    || AMPLIFY < tmpAmp - 0.1
  ) {
    AMPLIFY = tmpAmp;
    Serial.print("Amp: ");
    Serial.println(AMPLIFY, 3);

    #ifdef OLED
      char result[8];
      dtostrf(AMPLIFY, 6, 2, result); // Leave room for too large numbers!

      u8g2.firstPage(); 

      do{
        u8g2.setFont(u8g2_font_8x13_tr);
        u8g2.drawStr( 5, 1, "Amplify");
        u8g2.setFont(u8g2_font_10x20_tr);
        u8g2.drawStr( 5, 20, result);
      }while(u8g2.nextPage());
    #endif
  }
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

  if(PEAK_INDICATOR) {
    if(leftVal >= leftPeak) leftPeak = leftVal + 0.5f;   // +1 so indicator stays above level and +0.5 prevent flickering
    if(rightVal >= rightPeak) rightPeak = rightVal + 0.5f;

    // ensure to not exceed channel
    if(leftPeak > NUM_LEDS -1) leftPeak = NUM_LEDS -1;
    if(rightPeak > NUM_LEDS -1) rightPeak = NUM_LEDS -1;
  }
	
  my_cli();
  setLeds(leftCurrent, rightCurrent);
  delay(DELAY_MS);

  // decay
  if(leftCurrent > 0) leftCurrent -= CHANNEL_DECAY;
  if(rightCurrent > 0) rightCurrent -= CHANNEL_DECAY;

  if(PEAK_INDICATOR) {
    if(leftVal + 0.5f < leftPeak && leftPeak > 0) leftPeak -= PEAK_DECAY;
    if(rightVal + 0.5f < rightPeak && rightPeak > 0) rightPeak -= PEAK_DECAY;
  }
  
  #ifdef OLED
    //handle_menu();
  #endif
}
