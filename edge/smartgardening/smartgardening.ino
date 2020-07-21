#include <SPI.h>
#include <WiFiNINA.h>
#include "web.h"
#include "config.h"

bool ssid_set = false;

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
    apSetup();
  } else {
    //apSetup();
    Serial.println("Loaded config for access point: " + ssid + " with password length: " + password.length());
  }

}


void loop() {
  // Host access point
  while (!ssid_set && !apRun()) {}
  //apRun();
}
