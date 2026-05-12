#include <Arduino.h>

void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("Chip Model: " + String(ESP.getChipModel()));
  Serial.println("Chip Revision: " + String(ESP.getChipRevision()));
  Serial.println("Flash Size: " + String(ESP.getFlashChipSize() / (1024 * 1024)) + "MB");
}

void loop() {}