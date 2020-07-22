#ifndef WATERING_H
#define WATERING_H

#define MoisturePin A0
#define RelayPin 8
#define MoistureSendInterval 10L*1000L // 1 minute in millis

#include "config.h"

unsigned long lastSent = millis();
unsigned long lastSwitch = millis();
bool pumping = false;

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
}

void wateringLoop() {
  // Determine elapsed time
  unsigned long now = millis();

  // Read moisture
  int moisture = readMoisture();

  // Send moisture
  if ((now - lastSent) > MoistureSendInterval) {
    lastSent = now;
    sendMoisture(moisture);
  }

  // Turn pump ON
  if (!pumping && moisture < getMinHumidity()) {
    switchRelay(true);
    lastSwitch = now;
    pumping = true;
  }

  // Turn pump OFF
  unsigned long runTime = now - lastSwitch;
  if (pumping) {
    Serial.print("Runtime millis: ");
    Serial.println(runTime);
  }

  if (pumping && runTime > getMinWateringSec() * 1000 && (moisture > getMaxHumidity() || runTime > getMaxWateringSec() * 1000)) {
    switchRelay(false);
    pumping = false;
  }
}


#endif
