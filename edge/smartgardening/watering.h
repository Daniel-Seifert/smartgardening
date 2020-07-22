#ifndef WATERING_H
#define WATERING_H

#define MoisturePin A0
#define RelayPin 8

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
    digitalWrite(RelayPin, HIGH);
  } else {
    digitalWrite(RelayPin, LOW);
  }
}

void wateringLoop() {
  int moisture = readMoisture();
  Serial.print("Moisture: ");
  Serial.println(moisture);
  if (moisture < 50) {
    switchRelay(true);
  } else {
    switchRelay(false);
  }
}


#endif
