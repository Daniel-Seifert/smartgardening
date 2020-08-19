#ifndef WATERING_H
#define WATERING_H

// Pin usage
#define MoisturePin A0
#define RelayPin 8
#define TransmitPin 10
#define ReceivePin 2
#define Remote

// Timing settings
#define MoistureSendInterval 3L*60L*60L*1000L // 3 h
#define WeatherRequestInterval 3L*60L*60L*1000L // 3 hours
//#define MinWateringInterval 24L*60L*60L*1000L // 24h
#define MinWateringInterval 10L*1000L // 10s
#define ConfPullInterval 3L*60L*60L*1000L // 3h

// Codes for 433 MHz sending
#define A_ON 345425
#define A_OFF 345428
#define B_ON 348497
#define B_OFF 348500
#define C_ON 349265
#define C_OFF 349268
#define D_ON 349457
#define D_OFF 349460

// Includes
#include "api.h"
#include "config.h"
#include <RCSwitch.h>

// Global variables
unsigned long lastSent = millis();
unsigned long lastSwitch = millis();
unsigned long lastWeather = 0;
unsigned long lastConfPull = 0;
RCSwitch remoteSwitch = RCSwitch();

bool pumping = false;
bool raining = false;

// Functions
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
#if defined(Remote)
    for (int i = 0; i < 5; i++) {
      remoteSwitch.send(A_ON, 24);
      delay(200);
    }
#else
    digitalWrite(RelayPin, HIGH);
#endif
  } else {
    Serial.println("Turn pump: OFF");
#if defined(Remote)
    for (int i = 0; i < 5; i++) {
      remoteSwitch.send(A_OFF, 24);
      delay(200);
    }
#else
    digitalWrite(RelayPin, LOW);
#endif
  }
}

void setupPins() {
#if defined(Remote)
  remoteSwitch.enableReceive(ReceivePin);
  remoteSwitch.enableTransmit(TransmitPin);
#else
  pinMode(RelayPin, OUTPUT);
#endif
  switchRelay(false);
}

int receiveSignal() {
  int value = -1;
  if (remoteSwitch.available()) {
    value = remoteSwitch.getReceivedValue();
    remoteSwitch.resetAvailable();
  }
  return value;
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

  // Check for manual pumping signal
  int sig = receiveSignal();
  if (sig != -1) {
    switch(sig) {
      case A_ON: apiSetPumping(true); break;
      case A_OFF: apiSetPumping(false); break;
    }
  }
  
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
