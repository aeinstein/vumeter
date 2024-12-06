#ifndef MENU_H
#define MENU_H

#include "display.h"

int currentItem = -1;
int lastItem = -1;

const char *settings_list[] = {
    "Number of leds",
    "Peak decay",
    "Channel decay",
    "Brightness",
    "Glowness",
    "Mode",
    "Amplify",
    "Color High",
    "Color Mid",
    "Color Low",
    "Load Settings",
    "Save Settings"
};

int num_settings = sizeof(settings_list) / sizeof(char *);

void handle_menu(){
  if(currentItem == lastItem) return;

  char result[32];

  Serial.print("currentItem: ");
  Serial.println(currentItem, DEC);

  u8g2.firstPage();
  
  do{
    u8g2.setFont(u8g2_font_8x13_tr);
    u8g2.drawStr( 0, 0, settings_list[currentItem]);
    
    switch(currentItem){
      case 0:
        itoa(NUM_LEDS, result, 10);
        break;

      case 1:
        dtostrf(PEAK_DECAY, 6, 2, result);
        break;

      case 2:
        dtostrf(CHANNEL_DECAY, 6, 2, result);
        break;

      case 3:
        dtostrf(BRIGHTNESS, 6, 2, result);
        break;

      case 4:
        dtostrf(GLOWNESS, 6, 2, result);
        break;

      case 5:
        itoa(MODE, result, 10);
        break;

      case 6:
        dtostrf(AMPLIFY, 6, 2, result);
        break;

      case 7:
        break;

      case 8:
        break;

      case 9:
        break;

      case 10:
      case 11:
        strcpy(result, "Press right");
        break;

      default:
        break;
    }

    u8g2.setFont(u8g2_font_10x20_tr);
    u8g2.drawStr( 0, 20, result);

  }while(u8g2.nextPage());

  lastItem = currentItem;
}

void handle_menu_up(){
  if(currentItem < num_settings -1) currentItem++;
  handle_menu();
}

void handle_menu_down(){
  if(currentItem > 0) currentItem--;
  handle_menu();
}

void changeValue(int dir){
  switch(currentItem){
    case 0:
      if(dir) {
        if(NUM_LEDS < 144) NUM_LEDS++;
      } else {
        if(NUM_LEDS > 1) NUM_LEDS--;
      }
      
      break;

    case 1:
      if(dir) {
        if(PEAK_DECAY < 1) PEAK_DECAY += 0.02;
      } else {
        if(PEAK_DECAY > 0) PEAK_DECAY -= 0.02;
      }
      
      break;

    case 2:
      if(dir) {
        if(CHANNEL_DECAY < 1) CHANNEL_DECAY += 0.02;
      } else {
        if(CHANNEL_DECAY > 0) CHANNEL_DECAY -= 0.02;
      }
      
      break;

    default:
      break;
  }
}

void handle_menu_left(){
  changeValue(1);
  handle_menu();
}

void handle_menu_right(){
  changeValue(-1);
  handle_menu();
}

#endif
