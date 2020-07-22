#ifndef CONFIG_H
#define CONFIG_H

#include <EEPROM.h>
#include<string.h>

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
const int humid_min_bytes = 2;
const int humid_max_bytes = 2;
const int water_min_bytes = 2;
const int water_max_bytes = 2;
const int active_bytes = 1;
const int ssid_bytes = 32;
const int ssid_pw_bytes = 64;

/*
   ##############################################
   HELPER
   ##############################################
*/

void writeNulled(String value, int offset, int length) {
  for (int i = 0; i < length; i++) {
    if (i < value.length()) {
      EEPROM.write(offset + i, value.charAt(i));
    } else {
      EEPROM.write(offset + i, '\0');
    }
  }
}

String readString(int offset, int length) {
  String result = "";
  for (int i = 0; i < length; i++) {
    char next = EEPROM.read(offset + i);
    if (next != '\0') {
      result += next;
    } else {
      break;
    }
  }
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
  high = (wert >> 8) & 0xFF;
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

String getUuid () {
  return readString(uuid_offset, uuid_bytes);
}

int getMinHumidity () {
  return readInt(humid_min_offset);
}

int getMaxHumidity () {
  return readInt(humid_max_offset);
}

int getMinWateringSec () {
  return readInt(water_min_offset);
}

int getMaxWateringSec () {
  return readInt(water_max_offset);
}

bool isActive() {
  return readBool(active_offset);
}

String getSsid () {
  return readString(ssid_offset, ssid_bytes);
}

String getSsidPw () {
  return readString(ssid_pw_offset, ssid_pw_bytes);
}

/**
   EEPROM structure
   | UUID (16 bytes) [0-15]| Humid min (2 bytes) [16-17]| Humidity max (2 bytes) [18-19] |
   | wateringSec min (2 bytes) [20-21]| wateringSec max (2 bytes) [22-23]| active (1 byte) [24] |
   | SSID (32 bytes) [25-56] | SSID-Password (64 bytes) [57-110] |
*/

/*
   ##############################################
   SETTER
   ##############################################
*/

void storeUuid (String uuid) {
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

bool storeWifiData(String ssid, String password) {
  if (ssid.length() == 0 || password.length() == 0) {
    return false;
  }

  writeNulled(ssid, ssid_offset, ssid_bytes);
  writeNulled(password, ssid_pw_offset, ssid_pw_bytes);

  Serial.println("Stored SSID: " + ssid + ", password-length: " + password.length());
  return true;
}

#endif
