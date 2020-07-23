#ifndef WATERING_H
#define WATERING_H

#define MoisturePin A0
#define RelayPin 8
#define MoistureSendInterval 10L*60L*1000L // 10 m
#define WeatherRequestInterval 60L*60L*3L*1000L // 3 hours
#define MinWateringInterval 60L*1000L // 1m
#define ConfPullInterval 1L*60L*60L*1000L // 1 h

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
  // 100 % moisture == sensor analog output of 365
  moisture_value = map(moisture_value, 1023, 365, 0, 100);
  if (moisture_value > 100) {
    return 100;
  }
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
  if (lastWeather == 0 || (now - lastWeather) > WeatherRequestInterval) {
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
    Serial.println(runTime/1000);
  }

  // Turn pump OFF
  if (pumping && runTime > getMinWateringSec() * 1000 && (moisture > getMaxHumidity() || runTime > getMaxWateringSec() * 1000)) {
    switchRelay(false);
    pumping = false;
    apiSetPumping(false);
  }
}


#endif
