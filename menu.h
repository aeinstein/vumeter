#ifndef MENU_H
#define MENU_H

#include "display.h"

int currentItem = 0;
int lastItem = 0;
bool valSelected = false;

const char *settings_list[] = {
    "Number of leds",   // 0
    "Peak decay",       // 1
    "Peak indicator",   // 2
    "Channel decay",    // 3
    "Brightness",       // 4
    "Glowness",         // 5
    "Mode",             // 6
    #ifndef CALIBRATION_POTI
    "Amplify",          // 7
    #endif
    "Color Low",        // 8
    "Color Mid",        // 9
    "Color High",       // 10
    "Load Settings",    // 11
    "Save Settings"     // 12
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
      PEAK_INDICATOR = !PEAK_INDICATOR;
      break;

    case 3:
      if(increase) {
        CHANNEL_DECAY += 0.02;
        if(CHANNEL_DECAY >= 1) CHANNEL_DECAY = 0.01;
        
      } else {
        CHANNEL_DECAY -= 0.02;
        if(CHANNEL_DECAY < 0.01) CHANNEL_DECAY = 1;
      }

      break;

    case 4:
      if(increase) {
        BRIGHTNESS += 0.01;
        if(BRIGHTNESS > 1) BRIGHTNESS = 0.01;
        
      } else {
        BRIGHTNESS -= 0.01;
        if(BRIGHTNESS < 0.01) BRIGHTNESS = 1;
      }

      break;

    case 6:
      if(increase) {
        MODE++;
        if(MODE >4) MODE = 1;

      } else {
        MODE--;
        if(MODE < 1) MODE = 4;
      }
      break;

  #ifdef CALIBRATION_POTI
    case 7:
      if(increase) {
        AMPLIFY += 0.1;
        if(AMPLIFY >3) AMPLIFY = 3;

      } else {
        AMPLIFY -= 0.1;
        if(AMPLIFY < 0.1) AMPLIFY = 0.1;
      }
      break;

    case 11:
      if(increase) loadSettings();
      break;

    case 12:
      if(increase) saveSettings();
      break;
  #endif

  #ifndef CALIBRATION_POTI
    case 10:
      if(increase) loadSettings();
      break;

    case 11:
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
    u8g2.setDrawColor(1);
    u8g2.setFont(u8g2_font_8x13_tr);

    if(valSelected) {
      u8g2.drawLine(0, 15, 127, 15);
    } else {
      u8g2.drawBox(0, 0, 127, 15);
      u8g2.setDrawColor(0);
    }

    // Title
    u8g2.drawStr( ALIGN_CENTER(settings_list[currentItem]), 1, settings_list[currentItem]);

    switch(currentItem){
      case 0:
        itoa(NUM_LEDS, result, 10);
        break;

      case 1:
        dtostrf(PEAK_DECAY, 4, 2, result);
        break;

      case 2:
        if(PEAK_INDICATOR) strcpy(result, "ON");
        else strcpy(result, "OFF");
        break;

      case 3:
        dtostrf(CHANNEL_DECAY, 4, 2, result);
        break;

      case 4:
        dtostrf(BRIGHTNESS, 4, 2, result);
        break;

      case 5:
        sprintf(result, "%06X", GLOWNESS);
        break;

      case 6:
        itoa(MODE, result, 10);
        break;
  #ifndef CALIBRATION_POTI
      case 7:
        dtostrf(AMPLIFY, 4, 2, result);
        break;

      case 8:
        sprintf(result, "%06X", LC);
        break;

      case 9:
        sprintf(result, "%06X", MC);
        break;

      case 10:
        sprintf(result, "%06X", HC);
        break;

      case 11:
      case 12:
        strcpy(result, "Press right");
        break;
  #endif

  #ifdef CALIBRATION_POTI
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
      default:
        break;
    }

    u8g2.setDrawColor(1);

    if(valSelected) {
      u8g2.drawBox(0, 16, 127, 28);
      u8g2.setDrawColor(0);
    }
    
    u8g2.setFont(u8g2_font_10x20_tr);
    u8g2.drawStr(ALIGN_CENTER(result), 20, result);

    #ifdef CALIBRATION_POTI
      u8g2.setDrawColor(1);
      u8g2.drawLine(0, 45, 127, 45);
      u8g2.setFont(u8g2_font_8x13_tr);
      u8g2.drawStr( 5, 50, "Amplify:");
      dtostrf(AMPLIFY, 4, 2, result);
      u8g2.drawStr( 90, 50, result);
    #endif

  }while(u8g2.nextPage());

  lastItem = currentItem;
}

void handle_menu_left(){
  changeValue(0);
  handle_menu(true);
}

void handle_menu_right(){
  changeValue(1);
  handle_menu(true);
}

void handle_menu_up(){
  if(valSelected) {
    handle_menu_right();
    return;
  }

  currentItem++;
  if(currentItem >= num_settings) currentItem = 0;
  handle_menu();
}

void handle_menu_down(){
  if(valSelected) {
    handle_menu_left();
    return;
  }

  currentItem--;
  if(currentItem <= 0) currentItem = num_settings -1;
  handle_menu();
}

void handle_menu_click(){
  valSelected = !valSelected;
  handle_menu(true);
}

#endif
