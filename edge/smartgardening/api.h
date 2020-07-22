#ifndef API_H
#define API_H

#include "config.h"
#include <ArduinoJson.h>
#include <string.h>
#include "stringMethods.h"

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

void httpGet(char * endpoint) {
  apiConnect();

  char * sendMessage = malloc(sizeof(char) * (strlen(endpoint) + strlen(F("GET ")) + strlen(F(" HTTP/1.1")) + 1));
  sprintf(sendMessage, "GET %s HTTP/1.1", endpoint);
  Serial.println(sendMessage);
  api_client.println(sendMessage);
  api_client.println("Host: smart-gardening.herokuapp.com");
  api_client.println("Connection: close");
  if (api_client.println() == 0) {
    Serial.println("Failed to send request");
    return;
  }

  

  delay(2000);

  Byte *word = NULL;
  int wordlength;
  Serial.println(getWord(&api_client, &word, &wordlength));
  
//  while (api_client.available()) {
//    char c = api_client.read();
//    Serial.write(c);
//    //response.concat(c);
//  }
  
  Serial.println(*word);
  free(word);
  Serial.flush();

  //Serial.println(response);

  // Free stuff
  free(sendMessage);
}

void httpPost(char * endpoint, char * body) {
  apiConnect();
  char * sendMessage = malloc(sizeof(char) * (strlen(endpoint) + strlen(F("Post ")) + strlen(F(" HTTP/1.1")) + 1));
  sprintf(sendMessage, "POST %s HTTP/1.1", endpoint);
  Serial.print(F("Sending Message to enpoint: "));
  Serial.println(sendMessage);

  int content_length = strlen(body);


  api_client.println(sendMessage);
  api_client.println(F("Host: smart-gardening.herokuapp.com"));
  api_client.println(F("Connection: close"));
  api_client.print(F("Content-Length: "));
  api_client.println(content_length);
  api_client.println(F("Content-Type: application/json"));
  if (api_client.println() == 0) {
    Serial.println(F("Failed to send request"));
    return;
  }

  api_client.println(body);

  delay(2000);
  Byte *word = NULL;
  int wordlength;
  Serial.println(getWord(&api_client, &word, &wordlength));
  //  while (api_client.available()) {
  //    char c = api_client.read();
  //    Serial.write(c);
  //    //response.concat(c);
  //  }
  Serial.println(word);
  free(word);
  Serial.flush();

  //Serial.println(response);

  // Free stuff
  free(sendMessage);
}

void apiRegister() {
//    httpGet("/api/devices");
  httpPost("/edge/devices/register", "");
}

#endif
