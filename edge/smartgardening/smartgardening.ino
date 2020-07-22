#include <SPI.h>
#include <WiFiNINA.h>
#include "web.h"
#include "config.h"
#include "api.h"
#include "watering.h"

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

void printMemory() {
  Serial.print("Free Memory: ");
  Serial.println(freeMemory());
}

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("Starting setup...");
  printMemory();
  //clearEEPROM();
  //storeUuid("Empty");
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
  printMemory();
  free(ssid);
  free(password);
}

bool first = true;
void loop() {
  if (!wifi_connected) {
    Serial.println("Unable to connect to WIFI in main loop!");
  }

  char* uuid = getUuid();
  char* ssid = getSsid();
  char* password = getSsidPw();
  
  wifi_connected = connectWifi(ssid, password, 5);


//strlen(uuid) == 0

    printMemory();
    apiRegister();
    storeUuid("Unique");
    printMemory();
  

  printMemory();
  //wateringLoop();
  delay(1000);

  free(uuid);
  free(ssid);
  free(password);
}
