#include <ArduinoJson.h>

void setup() {
  Serial.begin(9600);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  StaticJsonDocument<300> doc;
  bool isPumping = true;
  doc["pumping"] = isPumping;
  Serial.println(doc.containsKey("pumping"));

}

void loop() {
  // put your main code here, to run repeatedly:

}
