#include <EEPROM.h>


struct MyConfig {
  #ifndef CALIBRATION_POTI
    float AMPLIFY;        // 4
  #endif

  unsigned int NUM_LEDS;  // 2
  unsigned long LC;       // 4
  unsigned long MC;       // 4
  unsigned long HC;       // 4

  float CHANNEL_DECAY;    // 4
  float PEAK_DECAY;       // 4
  float BRIGHTNESS;       // 4
  unsigned long GLOWNESS; // 4
  char SIGNATURE[3];
};


void saveSettings(){
  int addr = 0;

  MyConfig config = {
    #ifndef CALIBRATION_POTI
      AMPLIFY,
    #endif
    NUM_LEDS,
    LC,
    MC,
    HC,
    CHANNEL_DECAY,
    PEAK_DECAY,
    BRIGHTNESS,
    GLOWNESS,
    "MKB"
  };

  EEPROM.put(addr, config);

  Serial.print("config is ");
  Serial.print(sizeof(MyConfig), DEC);
  Serial.println(" bytes");
}

bool loadSettings(){
  int addr = 0;

  MyConfig config;

  EEPROM.get(addr, config);

  Serial.print("config read: ");
  Serial.print(sizeof(MyConfig), DEC);
  Serial.println(" bytes");


  Serial.print("Signature: ");
  Serial.println(config.SIGNATURE);

  if(strcmp(config.SIGNATURE, "MKB") == 0) {
    #ifndef CALIBRATION_POTI
      AMPLIFY = config.AMPLIFY;
    #endif

    NUM_LEDS = config.NUM_LEDS;
    
    Serial.print("NUM_LEDS: ");
    Serial.println(NUM_LEDS, DEC);

    LC = config.LC;
    MC = config.MC;
    HC = config.HC;

    Serial.print("LC: ");
    Serial.println(LC, HEX);

    Serial.print("MC: ");
    Serial.println(MC, HEX);

    Serial.print("HC: ");
    Serial.println(HC, HEX);

    CHANNEL_DECAY = config.CHANNEL_DECAY;
    PEAK_DECAY = config.PEAK_DECAY;
    BRIGHTNESS = config.BRIGHTNESS;
    GLOWNESS = config.GLOWNESS;

    Serial.print("CHANNEL_DECAY: "); 
    Serial.println(CHANNEL_DECAY, 3);

    Serial.print("PEAK_DECAY: "); 
    Serial.println(PEAK_DECAY, 3);

    Serial.print("BRIGHTNESS: "); 
    Serial.println(BRIGHTNESS, 3);

    Serial.print("GLOWNESS: "); 
    Serial.println(GLOWNESS, HEX);

    return 1;

  } else {
    Serial.println("Signature not match");
    return 0;
  }
}



