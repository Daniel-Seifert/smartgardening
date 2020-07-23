#ifndef API_H
#define API_H

#include "config.h"
#include <ArduinoJson.h>
#include <string.h>
#include "stringMethods.h"

WiFiSSLClient api_client;
char api_server[] = "smart-gardening.herokuapp.com";

bool connectWifi(char* ssid, char* password, int retries) {
  int count = 0;

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    return false;
  }

  while (WiFi.status() != WL_CONNECTED && count <= retries) {
    Serial.print("Attempt to connect to SSID: ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
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

  char * sendMessage = (char * )malloc(sizeof(char) * (strlen(endpoint) + strlen(F("GET ")) + strlen(F(" HTTP/1.1")) + 1));
  sprintf(sendMessage, "GET %s HTTP/1.1", endpoint);
  Serial.println(sendMessage);
  api_client.println(sendMessage);
  api_client.println("Host: smart-gardening.herokuapp.com");
  api_client.println("Connection: close");
  if (api_client.println() == 0) {
    Serial.println("Failed to send request");
    return NULL;
  }
  free(sendMessage);
  delay(2000);

  Byte *word = NULL;
  int wordlength = 0;
  Serial.println(getWord(&api_client, &word, &wordlength));
  Serial.println(word);
  free(word);
  Serial.flush();

  // Free stuff
  
}

char *  httpPost(const char * endpoint, const char * body) {
  apiConnect();
  
  char * sendMessage = (char *)malloc(sizeof(char) * (strlen(endpoint) + strlen(F("Post ")) + strlen(F(" HTTP/1.1")) + 1));
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
    return NULL;
  }

  // Free stuff
  free(sendMessage);
  delay(1000);
  api_client.println(body);
  char *word = NULL;
  int wordlength = 0;
  getWord(&api_client, &word, &wordlength);
  Serial.println(wordlength);
  
  return word;
}

void apiRegister() {
  char * response = httpPost("/edge/devices/register", "");
  StaticJsonDocument<300> doc;
  deserializeJson(doc, response);
  serializeJsonPretty(doc["id"], Serial);
  const char * data = doc["id"];
  storeUuid(data);
  free(response);
  char * uuid = getUuid();
  Serial.println(uuid);
  free(uuid);
}

#endif
