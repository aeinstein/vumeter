#ifndef DISPLAY_H
#define DISPLAY_H

#include <Wire.h>
#include <U8g2lib.h>

U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

void u8g2_prepare(void) {
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.setFontRefHeightExtendedText();
  u8g2.setDrawColor(1);
  u8g2.setFontPosTop();
  u8g2.setFontDirection(0);
}

uint8_t draw_state = 0;

bool display_setup() {
  Serial.println("OLED init");

  delay(150); // wait for the OLED to power up

  if(!u8g2.begin()) { 
    Serial.println(F("SSD1306 allocation failed"));
    return 0;
  }
  
  u8g2_prepare();

  Serial.println("OLED inited");
  return 1;
}

#endif
