#ifndef CONFIG_H
#define CONFIG_H

#include <EEPROM.h>
#include<string.h>

/**
   EEPROM structure
   | UUID (16 bytes) [0-15]| Humid min (4 bytes) [16-19]| Humidity max (4 bytes) [20-23] |
   | wateringSec min (4 bytes) [24-27]| wateringSec max (4 bytes) [28-31]| active (1 byte) [32] |
   | SSID (32 bytes) [33-64] | SSID-Password (64 bytes) [65-128] |
*/

const int uuid_offset = 0;
const int humid_min_offset = 16;
const int humid_max_offset = 20;
const int water_min_offset = 24;
const int water_max_offset = 28;
const int active_offset = 32;
const int ssid_offset = 33;
const int ssid_pw_offset = 65;

const int uuid_bytes = 16;
const int humid_min_bytes = 4;
const int humid_max_bytes = 4;
const int water_min_bytes = 4;
const int water_max_bytes = 4;
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

/*
   ##############################################
   GETTER
   ##############################################
*/

String getUuid () {
  return readString(uuid_offset, uuid_bytes);
}

String getSsid () {
  return readString(ssid_offset, ssid_bytes);
}

String getSsidPw () {
  return readString(ssid_pw_offset, ssid_pw_bytes);
}


/*
   ##############################################
   SETTER
   ##############################################
*/

int storeWifiData(String ssid, String password) {
  if (ssid.length() == 0 || password.length() == 0) {
    return 0;
  }

  writeNulled(ssid, ssid_offset, ssid_bytes);
  writeNulled(password, ssid_pw_offset, ssid_pw_bytes);

  Serial.println("Stored SSID: " + ssid + ", password-length: " + password.length());
  return 1;
}


#endif
