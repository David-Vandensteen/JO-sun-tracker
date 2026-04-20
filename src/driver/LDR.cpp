#include "LDR.h"
#include "settings.h"

LDR::LDR() : raw(0), percent(0) {}

void LDR::read(uint8_t pin, int analogResolution) {
  raw = analogRead(pin);
  percent = map(raw, 0, analogResolution, 100, 0);
}

void LDRs::read(uint8_t pinUp, uint8_t pinDown, uint8_t pinNight, int analogResolution) {
  dayUp.read(pinUp, analogResolution);
  dayDown.read(pinDown, analogResolution);
  night.read(pinNight, analogResolution);
}
