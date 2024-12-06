#ifndef MENU_H
#define MENU_H

#include "display.h"

int currentItem = 0;
int lastItem = 0;

const char *settings_list[] = {
    "Number of leds",
    "Peak decay",
    "Channel decay",
    "Brightness",
    "Glowness",
    "Mode",
    #ifndef CALIBRATION_POTI
    "Amplify",
    #endif
    "Color Low",
    "Color Mid",
    "Color High",
    "Load Settings",
    "Save Settings"
};

int num_settings = sizeof(settings_list) / sizeof(char *);

void changeValue(bool increase = true){
  switch(currentItem){
    case 0:
      if(increase) {
        NUM_LEDS++;
        if(NUM_LEDS > 144) NUM_LEDS = 1;

      } else {
        NUM_LEDS--;
        if(NUM_LEDS < 1) NUM_LEDS = 144;
      }
      
      break;

    case 1:
      if(increase) {
        PEAK_DECAY += 0.02;
        if(PEAK_DECAY >= 1) PEAK_DECAY = 0.01;

      } else {
        PEAK_DECAY -= 0.02;
        if(PEAK_DECAY < 0.01) PEAK_DECAY = 1;
      }
      
      break;

    case 2:
      if(increase) {
        CHANNEL_DECAY += 0.02;
        if(CHANNEL_DECAY >= 1) CHANNEL_DECAY = 0.01;
        
      } else {
        CHANNEL_DECAY -= 0.02;
        if(CHANNEL_DECAY < 0.01) CHANNEL_DECAY = 1;
      }

      break;

    case 3:
      if(increase) {
        BRIGHTNESS += 0.01;
        if(BRIGHTNESS > 1) BRIGHTNESS = 0.01;
        
      } else {
        BRIGHTNESS -= 0.01;
        if(BRIGHTNESS < 0.01) BRIGHTNESS = 1;
      }

      break;

    case 5:
      if(increase) {
        MODE++;
        if(MODE >4) MODE = 1;

      } else {
        MODE--;
        if(MODE < 1) MODE = 4;
      }
      break;

  #ifdef CALIBRATION_POTI
    case 6:
      if(increase) {
        AMPLIFY += 0.1;
        if(AMPLIFY >3) AMPLIFY = 3;

      } else {
        AMPLIFY -= 0.1;
        if(AMPLIFY < 0.1) AMPLIFY = 0.1;
      }
      break;


    case 10:
      if(increase) loadSettings();
      break;

    case 11:
      if(increase) saveSettings();
      break;
  #endif

  #ifndef CALIBRATION_POTI
    case 9:
      if(increase) loadSettings();
      break;

    case 10:
      if(increase) saveSettings();
      break;
  #endif

    default:
      break;
  }
}

void handle_menu(bool force = false){
  if(!force && currentItem == lastItem) return;

  char result[32];

  Serial.print("currentItem: ");
  Serial.println(currentItem, DEC);

  u8g2.firstPage();
  
  do{
    u8g2.setFont(u8g2_font_8x13_tr);
    u8g2.drawStr( 5, 0, settings_list[currentItem]);
    u8g2.drawLine(0, 15, 127, 15);

    switch(currentItem){
      case 0:
        itoa(NUM_LEDS, result, 10);
        break;

      case 1:
        dtostrf(PEAK_DECAY, 4, 2, result);
        break;

      case 2:
        dtostrf(CHANNEL_DECAY, 4, 2, result);
        break;

      case 3:
        dtostrf(BRIGHTNESS, 4, 2, result);
        break;

      case 4:
        sprintf(result, "%06X", GLOWNESS);
        break;

      case 5:
        itoa(MODE, result, 10);
        break;
  #ifndef CALIBRATION_POTI
      case 6:
        dtostrf(AMPLIFY, 4, 2, result);
        break;

      case 7:
        sprintf(result, "%06X", LC);
        break;

      case 8:
        sprintf(result, "%06X", MC);
        break;

      case 9:
        sprintf(result, "%06X", HC);
        break;

      case 10:
      case 11:
        strcpy(result, "Press right");
        break;
  #endif

  #ifdef CALIBRATION_POTI
      case 6:
        sprintf(result, "%06X", LC);
        break;

      case 7:
        sprintf(result, "%06X", MC);
        break;

      case 8:
        sprintf(result, "%06X", HC);
        break;

      case 9:
      case 10:
        strcpy(result, "Press right");
        break;

  #endif
      default:
        break;
    }

    u8g2.setFont(u8g2_font_10x20_tr);
    u8g2.drawStr( 5, 20, result);

    #ifdef CALIBRATION_POTI
      u8g2.drawLine(0, 45, 127, 45);
      u8g2.setFont(u8g2_font_8x13_tr);
      u8g2.drawStr( 5, 50, "Amplify:");
      dtostrf(AMPLIFY, 4, 2, result);
      u8g2.drawStr( 80, 50, result);
    #endif

  }while(u8g2.nextPage());

  lastItem = currentItem;
}

void handle_menu_up(){
  currentItem++;
  if(currentItem >= num_settings) currentItem = 0;
  handle_menu();
}

void handle_menu_down(){
  currentItem--;
  if(currentItem <= 0) currentItem = num_settings -1;
  handle_menu();
}

void handle_menu_left(){
  changeValue(0);
  handle_menu(true);
}

void handle_menu_right(){
  changeValue(1);
  handle_menu(true);
}

#endif
