#include <SPI.h>
#include <WiFiNINA.h>

const char* ssid = "AndroidAP7a8f";
const char* pass = "vcxr6631";
WiFiSSLClient client;

//char server[] = "www.google.com";
char server[] = "smart-gardening.herokuapp.com";

int status = WL_IDLE_STATUS;

void setup() {
  Serial.begin(19200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println(WIFI_FIRMWARE_LATEST_VERSION);
    Serial.println("Please upgrade the firmware");
  }

  
  Serial.println("ficken");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }

  Serial.println("lol");

  if (client.connect(server, 443)) {
    Serial.println("connected to server");
    // Make a HTTP request:
//    client.println("GET /search?q=arduino HTTP/1.1");
//    client.println("Host: www.google.com");
//    client.println("Connection: close");
    client.println("GET /api/devices HTTP/1.1");
    client.println("Host: smart-gardening.herokuapp.com");
    client.println("Connection: close");
    client.println();
  }

}

void loop() {
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }

}
