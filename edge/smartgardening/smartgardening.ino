#include <SPI.h>
#include <WiFiNINA.h>
#include "web.h"
#include "config.h"
#include "api.h"
#include "watering.h"

bool ssid_set = false;
bool wifi_connected = false;

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("Starting setup...");
  //clearEEPROM();
  //storeUuid("");
  printConfig();

  char* ssid = getSsid();
  char* password = getSsidPw();
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
  } else {
    Serial.print("Loaded config for access point: ");
    Serial.print(ssid);
    Serial.print(" with password length: ");
    Serial.println(strlen(password));
  }

  wifi_connected = connectWifi(ssid, password, 5);
  if (!wifi_connected) {
    Serial.println("Unable to connect to wifi!");
  }
  Serial.println("Finished setup");
  free(ssid);
  free(password);
}
int counterValue = 0;

void loop() {
  if (!wifi_connected) {
    Serial.println("Unable to connect to WIFI in main loop!");
  }

  char *uuid = getUuid();
  char *ssid = getSsid();
  char *password = getSsidPw();

  wifi_connected = connectWifi(ssid, password, 5);
  free(password);
  free(ssid);

  if (strlen(uuid) == 0) {
    Serial.println(uuid);
    free(uuid);
    apiRegister();
  }else {
    free(uuid);
  }

  //wateringLoop();
//  apiPostMeasurement(counterValue++);
  if (counterValue%2 == 0) {
    apiSetPumping(true);
    
  }
  else {
    apiSetPumping(false);
  }
  counterValue++;
  printConfig();
  
  delay(5000);
}
