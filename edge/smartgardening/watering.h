#ifndef WATERING_H
#define WATERING_H

#define MoisturePin A0
#define RelayPin 8
#define MoistureSendInterval 3L*60L*60L*1000L // 3 h
#define WeatherRequestInterval 3L*60L*60L*1000L // 3 hours
#define MinWateringInterval 24L*60L*60L*1000L // 24h
#define ConfPullInterval 3L*60L*60L*1000L // 3h

#include "api.h"
#include "config.h"

unsigned long lastSent = millis();
unsigned long lastSwitch = millis();
unsigned long lastWeather = 0;
unsigned long lastConfPull = 0;

bool pumping = false;
bool raining = false;

int readMoisture() {
  int moisture_value = analogRead(MoisturePin);
  // 100 % moisture == sensor analog output of 427

  // Map value
  moisture_value = map(moisture_value, 816, 427, 0, 100);
  if (moisture_value > 100) {
    return 100;
  }

  // Clamp value
  if (moisture_value < 0)
    moisture_value = 0;
  if (moisture_value > 100)
    moisture_value = 100;

  return moisture_value;
}

void switchRelay(bool value) {
  if (value) {
    Serial.println("Turn pump: ON");
    digitalWrite(RelayPin, HIGH);
  } else {
    Serial.println("Turn pump: OFF");
    digitalWrite(RelayPin, LOW);
  }
}

void setupPins() {
  pinMode(RelayPin, OUTPUT);
  switchRelay(false);
}

void sendMoisture(int moisture) {
  Serial.print("Sending Moisture: ");
  Serial.print(moisture);
  Serial.println("%");
  apiPostMeasurement(moisture);
}

void pullConfig() {
  Serial.println("Pull config");
  apiGetConfig();
}

void requestWeather() {
  Serial.println("Request weather ");
  raining = apiIsRaining();
}

void wateringLoop() {
  // Determine elapsed time
  unsigned long now = millis();

  // Read moisture
  int moisture = readMoisture();

  // Read configs
  if (lastConfPull == 0 || (now - lastConfPull) > ConfPullInterval) {
    pullConfig();
    lastConfPull = now;
  }
  bool outdoor = isOutdoor();

  // Request weather
  if (outdoor && (lastWeather == 0 || (now - lastWeather) > WeatherRequestInterval)) {
    requestWeather();
    lastWeather = now;
  }

  // Send moisture
  if ((now - lastSent) > MoistureSendInterval) {
    lastSent = now;
    sendMoisture(moisture);
  }

  // Turn pump ON
  if (!pumping && (now - lastSwitch) > MinWateringInterval && moisture < getMinHumidity() && !(outdoor && raining)) {
    switchRelay(true);
    lastSwitch = now;
    pumping = true;
    apiSetPumping(true);
  }

  // Show run time
  unsigned long runTime = now - lastSwitch;
  if (pumping) {
    Serial.print("Runtime sec: ");
    Serial.println(runTime / 1000);
  }

  // Turn pump OFF
  if (pumping && runTime > getMinWateringSec() * 1000 && (moisture > getMaxHumidity() || runTime > getMaxWateringSec() * 1000)) {
    switchRelay(false);
    pumping = false;
    apiSetPumping(false);
  }
}


#endif
