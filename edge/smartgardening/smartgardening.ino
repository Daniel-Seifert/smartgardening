#include <SPI.h>
#include <WiFiNINA.h>
#include "web.h"
#include "config.h"
#include "api.h"

bool ssid_set = false;
bool wifi_connected = false;

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

void printMemory() {
  Serial.print("Free Memory: ");
  Serial.println(freeMemory());
}

int count = 0;

void loop() {
  if (!wifi_connected) {
    Serial.println("Unable to connect to WIFI in main loop!");
  }

  String ssid = getSsid();
  String password = getSsidPw();
  wifi_connected = connectWifi(ssid, password, 5);
  if (count < 1) {
    apiRegister();
    count++;
  }
  
  printMemory();
  delay(5000);
}
