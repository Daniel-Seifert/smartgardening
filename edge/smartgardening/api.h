#ifndef API_H
#define API_H

#include "config.h"
#include <ArduinoJson.h>
#include <string.h>
#include "stringMethods.h"

const int bufferSize = 300;

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

void httpRequest(const char * TYPE, const char * endpoint, const  char * body) {
  apiConnect();
  char * sendMessage = (char *)malloc(sizeof(char) * (strlen(TYPE) + strlen(endpoint) + strlen(F(" HTTP/1.1")) + 1));
  sprintf(sendMessage, "%s %s HTTP/1.1",TYPE, endpoint);
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
  api_client.println(body);
  // Free stuff
  free(sendMessage);
  delay(500);
}

void httpGet(char * endpoint) {
  httpRequest("GET", endpoint, "");
}

void httpPut(const char * endpoint, const char * body){
  httpRequest("PUT", endpoint, body);
}

void httpPost(const char * endpoint, const char * body) {
  httpRequest("POST", endpoint, body);
}

char * readClient() {
  char *word = NULL;
  int wordlength = 0;
  getWord(&api_client, &word, &wordlength);
  return word;
}

void apiRegister() {
  httpPost("/edge/devices/register", "");
  char * response = readClient();
  
  StaticJsonDocument<bufferSize> doc;
  deserializeJson(doc, response);
  Serial.print("Setting uuid: ");
  serializeJsonPretty(doc["id"], Serial);
  Serial.println();
  const char * data = doc["id"];
  storeUuid(data);
  free(response);
}

void apiPostMeasurement(int value) {
  char * endpoint = mallocString(sizeof(char) * (strlen("/edge/devices/") + strlen("/measures") + uuid_bytes + 1));
  char * uuid = getUuid();
  char * body = mallocString(sizeof(char) * bufferSize);
  sprintf(endpoint, "/edge/devices/%s/measures", uuid);
  StaticJsonDocument<bufferSize> doc;
  doc["value"] = value;
  doc["measureType"] = "MOISTURE";
  serializeJson(doc, body, bufferSize);
  
  Serial.print("apiPostMeasurement sending body: ");
  Serial.println(body);
  httpPost(endpoint, body);
  free(body);
  free(uuid);
  free(endpoint);  

  char * response = readClient();
  deserializeJson(doc, response);
  Serial.print("Sent measurment. Response: ");
  serializeJsonPretty(doc, Serial);
  Serial.println();
  free(response);
}

void apiSetPumping(bool isPumping) {
  char * endpoint = mallocString(sizeof(char) * (strlen("/edge/devices/") + strlen("/status") + uuid_bytes + 1));
  char * uuid = getUuid();
  char * body = mallocString(sizeof(char) * bufferSize);
  sprintf(endpoint, "/edge/devices/%s/status", uuid);
  StaticJsonDocument<bufferSize> doc;
  doc["pumping"] = isPumping;
  
  serializeJson(doc, body, bufferSize);

  Serial.print("apiPutPumping sending body: ");
  Serial.println(body);
  httpPut(endpoint, body);
  free(body);
  free(uuid);
  free(endpoint);

  char * response = readClient();
  deserializeJson(doc, response);
  Serial.print("Sent pumping. Response: ");
  serializeJsonPretty(doc, Serial);
  Serial.println();
  free(response);
}

void apiGetConfig() {
  char * endpoint = mallocString(sizeof(char) * (strlen("/edge/devices/") + strlen("/config") + uuid_bytes + 1));
  char * uuid = getUuid();
  sprintf(endpoint, "/edge/devices/%s/config", uuid);
  httpGet(endpoint);
  free(endpoint);
  free(uuid);

  char * response = readClient();
  StaticJsonDocument<bufferSize> doc;
  deserializeJson(doc, response);
  Serial.print("Got config: ");
  serializeJsonPretty(doc, Serial);
  Serial.println();

  storeMinHumidity(doc["minHumidity"]);
  storeMaxHumidity(doc["maxHumidity"]);
  storeMinWateringSec(doc["minWateringSeconds"]);
  storeMaxWateringSec(doc["maxWateringSeconds"]);
  storeActive(doc["activated"]);
  storeOutdoor(doc["outdoor"]);
  free(response);
}

bool apiIsRaining() {
  char * endpoint = mallocString(sizeof(char) * (strlen("/edge/devices/") + strlen("/weather") + uuid_bytes + 1));
  char * uuid = getUuid();
  sprintf(endpoint, "/edge/devices/%s/weather", uuid);
  httpGet(endpoint);
  free(endpoint);
  free(uuid);

  char * response = readClient();
  StaticJsonDocument<bufferSize> doc;
  deserializeJson(doc, response);
  Serial.print("Got WeatherData: ");
  serializeJsonPretty(doc, Serial);
  Serial.println();
  bool rain = doc["rain"];
  free(response);
  return rain;
}

#endif
