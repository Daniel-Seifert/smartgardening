#ifndef CONFIG_H
#define CONFIG_H

#include <EEPROM.h>
#include<string.h>


#include "stringMethods.h"

/**
   EEPROM structure
   | UUID (16 bytes) [0-15]| Humid min (2 bytes) [16-17]| Humidity max (2 bytes) [18-19] |
   | wateringSec min (2 bytes) [20-21]| wateringSec max (2 bytes) [22-23]| active (1 byte) [24] |
   | SSID (32 bytes) [25-56] | SSID-Password (64 bytes) [57-110] |
*/

const int uuid_offset = 0;
const int humid_min_offset = 16;
const int humid_max_offset = 18;
const int water_min_offset = 20;
const int water_max_offset = 22;
const int active_offset = 24;
const int ssid_offset = 25;
const int ssid_pw_offset = 57;
const int uuid_bytes = 16;
const int ssid_bytes = 32;
const int ssid_pw_bytes = 64;

// defaults
int min_humidity_default = 30;
int max_humidity_default = 70;
int min_watering_sec_default = 5;
int max_watering_sec_default = 10;

#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif  // __arm__

int freeMemory() {
  char top;
#ifdef __arm__
  return &top - reinterpret_cast<char*>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
  return &top - __brkval;
#else  // __arm__
  return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif  // __arm__
}

void printMemorys() {
  Serial.print("Free Memory: ");
  Serial.println(freeMemory());
}

/*
   ##############################################
   HELPER
   ##############################################
*/

void writeNulled(char* value, int offset, int length) {
  for (int i = 0; i < length; i++) {
    if (i < strlen(value)) {
      EEPROM.write(offset + i, value[i]);
    } else {
      EEPROM.write(offset + i, '\0');
    }
  }
}

char * readString(int offset, int length) {
  char * result = (char *)malloc(sizeof(char) * length + 1);
  printMemorys();
  if (result == NULL) {
    Serial.println("Error malloc");
  }

  for (int i = 0; i < length; i++) {
    char next = EEPROM.read(offset + i);
    if (next != 0) {
      result[i] = next;
    } else {
      result[i] = 0;
      return result;
    }
  }
  result[length] = 0;
  return result;
}

int readInt(int offset) {
  byte low, high;
  low = EEPROM.read(offset);
  high = EEPROM.read(offset + 1);
  return low + ((high << 8) & 0xFF00);
}

void writeInt(int offset, int value) {
  byte low, high;
  low = value & 0xFF;
  high = (value >> 8) & 0xFF;
  EEPROM.write(offset, low);
  EEPROM.write(offset + 1, high);
  return;
}

bool readBool(int offset) {
  byte data = EEPROM.read(offset);
  return data == 1;
}

void writeBool(int offset, bool value) {
  if (value) {
    EEPROM.write(offset, 1);
  } else {
    EEPROM.write(offset, 0);
  }
}

/*
   ##############################################
   GETTER
   ##############################################
*/

int getMinHumidity () {
  int value = readInt(humid_min_offset);
  if (value == 0) {
    return min_humidity_default;
  }
  return value;
}

int getMaxHumidity () {
  int value = readInt(humid_max_offset);
  if (value == 0) {
    return max_humidity_default;
  }
  return value;
}

int getMinWateringSec () {
  int value = readInt(water_min_offset);
  if (value == 0) {
    return min_watering_sec_default;
  }
  return value;
}

int getMaxWateringSec () {
  int value = readInt(water_max_offset);
  if (value == 0) {
    return max_watering_sec_default;
  }
  return value;
}

bool isActive() {
  return readBool(active_offset);
}

char* getUuid () {
  return readString(uuid_offset, uuid_bytes);
}

char* getSsid () {
  return readString(ssid_offset, ssid_bytes);
}

char* getSsidPw () {
  return readString(ssid_pw_offset, ssid_pw_bytes);
}

/*
   ##############################################
   UTIL
   ##############################################
*/

void clearEEPROM() {
  Serial.println("###### CLEAR EEPROM ######");
  for (int i = 0; i < EEPROM.length(); i++) {
    EEPROM.write(i, 0);
  }
}

void printConfig() {
  char* uuid = getUuid();

  Serial.println("Config from EEPROM ##############");
  Serial.print("UUID: ");
  Serial.println(uuid);
  Serial.print("min humidity: ");
  Serial.println(getMinHumidity());
  Serial.print("max humidity: ");
  Serial.println(getMaxHumidity());
  Serial.print("min watering sec: ");
  Serial.println(getMinWateringSec());
  Serial.print("max watering sec: ");
  Serial.println(getMaxWateringSec());
  Serial.print("Is Active: ");
  Serial.println(isActive());
  Serial.println("Config from EEPROM ##############");

  free(uuid);
}

/*
   ##############################################
   SETTER
   ##############################################
*/

void storeUuid (const char * uuid) {
  writeNulled(uuid, uuid_offset, uuid_bytes);
}

void storeMinHumidity (int min_humidity) {
  writeInt(humid_min_offset, min_humidity);
}

void storeMaxHumidity (int max_humidity) {
  writeInt(humid_max_offset, max_humidity);
}

void storeMinWateringSec (int min_watering) {
  writeInt(water_min_offset, min_watering);
}

void storeMaxWateringSec (int max_watering) {
  writeInt(water_max_offset, max_watering);
}

void storeActive(bool value) {
  writeBool(active_offset, value);
}

bool storeWifiData(char* ssid, char* password) {
  if (strlen(ssid) == 0 || strlen(password) == 0) {
    return false;
  }

  writeNulled(ssid, ssid_offset, ssid_bytes);
  writeNulled(password, ssid_pw_offset, ssid_pw_bytes);

  Serial.print("Stored SSID: ");
  Serial.print(ssid);
  Serial.print(" with password length: ");
  Serial.println(strlen(password));
  return true;
}

#endif
