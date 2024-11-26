#include <EEPROM.h>

unsigned int writeFloatIntoEEPROM(int address, float number){
  EEPROM.put(address, number);
  return address +4;
}

unsigned int readFloatFromEEPROM(int address, float number){
  EEPROM.get(address, number);
  return address+4;
}

unsigned int writeColorIntoEEPROM(int address, int color[]){
  EEPROM.write(address++, color[0]);
  EEPROM.write(address++, color[1]);
  EEPROM.write(address++, color[2]);
  return address +3;
}

unsigned int readColorFromEEPROM(int address, int color[]){
	color[0] = EEPROM.read(address++);
	color[1] = EEPROM.read(address++);
	color[2] = EEPROM.read(address++);
	return address +3;
}

unsigned int readIntFromEEPROM(int address, int number){
  byte byte1 = EEPROM.read(address);
  byte byte2 = EEPROM.read(address + 1);
  number = (byte1 << 8) + byte2;
  return address +2;
}

unsigned int writeIntIntoEEPROM(int address, int number){
  byte byte1 = number >> 8;
  byte byte2 = number & 0xFF;
  EEPROM.write(address, byte1);
  EEPROM.write(address + 1, byte2);

  return address +2;
}

unsigned int writeUnsignedIntIntoEEPROM(int address, unsigned int number) {
  EEPROM.write(address, number >> 8);
  EEPROM.write(address + 1, number & 0xFF);
  return address +2;
}

unsigned int readUnsignedIntFromEEPROM(int address, unsigned int number){
  number = (EEPROM.read(address) << 8) + EEPROM.read(address + 1);
  return address +2;
}

bool hasStoredConfig(){
	if(EEPROM.read(39) == 77 && EEPROM.read(40) == 75) return true;
	return false;
}

void saveSettings(){
  int addr = 0;

  #ifndef CALIBRATION_POTI
    addr = writeFloatIntoEEPROM(addr, AMPLIFY);
  #endif

  addr = writeUnsignedIntIntoEEPROM(addr, NUM_LEDS);

  addr = writeColorIntoEEPROM(addr, lowColor);
  addr = writeColorIntoEEPROM(addr, midColor);
  addr = writeColorIntoEEPROM(addr, highColor);

  addr = writeFloatIntoEEPROM(addr, CHANNEL_DECAY);
  addr = writeFloatIntoEEPROM(addr, PEAK_DECAY);
  addr = writeFloatIntoEEPROM(addr, BRIGHTNESS);
  addr = writeUnsignedIntIntoEEPROM(addr, GLOWNESS);

  Serial.print("config is ");
  Serial.print(addr, DEC);
  Serial.println(" bytes");

  // Config Signature
  EEPROM.write(39, 77);
  EEPROM.write(40, 75);
}

void loadSettings(){
  int addr = 0;

  #ifndef CALIBRATION_POTI
    addr = readFloatFromEEPROM(addr, AMPLIFY);
  #endif

  addr = readUnsignedIntFromEEPROM(addr, NUM_LEDS);
  addr = readColorFromEEPROM(addr, lowColor);
  addr = readColorFromEEPROM(addr, midColor);
  addr = readColorFromEEPROM(addr, highColor);

  addr = readFloatFromEEPROM(addr, CHANNEL_DECAY);
  addr = readFloatFromEEPROM(addr, PEAK_DECAY);
  addr = readFloatFromEEPROM(addr, BRIGHTNESS);
  
  addr = readUnsignedIntFromEEPROM(addr, GLOWNESS);

  Serial.print("config read: ");
  Serial.print(addr, DEC);
  Serial.println(" bytes");
}
