#include <SPI.h>
#include <WiFiNINA.h>
#include "web.h"
#include "config.h"
#include "api.h"
#include "watering.h"
#include "mem.h"

#define N0DEBUG

const int resetThreshold = 3;

bool ssid_set = false;
bool wifi_connected = false;

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  //clearEEPROM();
  Serial.println("Starting setup...");
  printConfig();

  char *ssid = getSsid();
  char *password = getSsidPw();
  ssid_set = strlen(ssid) > 0 && strlen(password) > 0;
  if (!ssid_set) {
    // Setup access point
    while (!apSetup()) {}
    // Host access point
    while (!apRun()) {}
    free(ssid);
    free(password);

    // Write back ssid and password from user input
    ssid = getSsid();
    password = getSsidPw();
  }
  else {
    Serial.print("Loaded config for access point: ");
    Serial.print(ssid);
    Serial.print(" with password length: ");
    Serial.println(strlen(password));
  }

  Serial.println("Connecting to Wifi");
  wifi_connected = connectWifi(ssid, password, 5);
  if (!wifi_connected) {
    int reset = getReset();
    reset++;
    if (reset >= resetThreshold) {
      Serial.println("Reset Arduino. Clearing EEPROM");
      clearEEPROM();
      Serial.println("Restarting");
      resetFunc();
    }
    storeReset(reset);
    Serial.println("Unable to connect to wifi!");
    Serial.print("Reset counter is now: ");
    Serial.println(reset);
    Serial.print("Arduino does a config reset when reset counter reaches: ");
    Serial.println(resetThreshold);
  }


  setupPins();
  Serial.println("Finished setup");
  free(ssid);
  free(password);
}

void loop() {
  char *ssid = getSsid();
  char *password = getSsidPw();

  unsigned long now = millis();
  if ((now - lastConnectionTry) > WifiReconnectInterval) {
    lastConnectionTry = now;
    wifi_connected = connectWifi(ssid, password, 3);
  }
  free(password);
  free(ssid);
  char *uuid = getUuid();
  if (strlen(uuid) == 0) {
    Serial.println(uuid);
    free(uuid);
    apiRegister();
  }
  else {
    free(uuid);
  }

#ifndef N0DEBUG
  printMemory();
#endif
  wateringLoop();
  delay(100);
}
