#include <SPI.h>
#include <WiFiNINA.h>
#include "web.h"
#include "config.h"
#include "api.h"

bool ssid_set = false;
bool wifi_connected = false;

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  String ssid = getSsid();
  String password = getSsidPw();
  ssid_set = ssid.length() > 0 && password.length() > 0;
  if (!ssid_set) {
    // Setup access point
    while (!apSetup()) {}
    // Host access point
    while (!apRun()) {}
    // Write back ssid and password from user input
    ssid = getSsid();
    password = getSsidPw();
  } else {
    Serial.println("Loaded config for access point: " + ssid + " with password length: " + password.length());
  }

  wifi_connected = connectWifi(ssid, password, 5);
  if (!wifi_connected) {
    Serial.println("Unable to connect to wifi!");
  }
  apiConnect();
}


void loop() {
  if (!wifi_connected) {
    Serial.println("Unable to connect to WIFI in main loop!");
  }

  String ssid = getSsid();
  String password = getSsidPw();
  wifi_connected = connectWifi(ssid, password, 5);
  apiRegister();
  delay(5000);
}
