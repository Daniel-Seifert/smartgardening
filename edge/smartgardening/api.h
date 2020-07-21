#ifndef API_H
#define API_H

#include "config.h"
#include <ArduinoJson.h>

WiFiSSLClient api_client;
char api_server[] = "smart-gardening.herokuapp.com";

bool connectWifi(String ssid, String password, int retries) {
  int count = 0;

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    return false;
  }

  while (WiFi.status() != WL_CONNECTED && count <= retries) {
    Serial.println("Attempt to connect to SSID: " + ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network
    WiFi.begin(ssid.c_str(), password.c_str());
  }
  return WiFi.status() == WL_CONNECTED;
}

void apiConnect() {
  if (!api_client.connected()) {
    Serial.println("Connecting client");
    api_client.connect(api_server, 443);
  }
}

void httpGet(String endpoint) {
  apiConnect();
  //String response = "";
  Serial.println("GET " + endpoint + " HTTP/1.1");
  api_client.println("GET /api/devices HTTP/1.1");
  api_client.println("Host: smart-gardening.herokuapp.com");
  api_client.println("Connection: close");
  if (api_client.println() == 0) {
    Serial.println("Failed to send request");
    return;
  }

  delay(2000);
  while (api_client.available()) {
    char c = api_client.read();
    Serial.write(c);
    //response.concat(c);
  }
  Serial.flush();

  //Serial.println(response);
}

String apiRegister() {
  httpGet("/api/devices");

}

#endif
