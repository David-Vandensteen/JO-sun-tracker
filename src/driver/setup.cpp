#include <Arduino.h>
#include "settings.h"
#include "setup.h"

void Setup::pin(SettingsBoardPin pin) {
  for (uint8_t i = 0; i < TRACKER_MAX; i++) {
    pinMode(pin.tracker[i].ldr.day.up, INPUT);
    pinMode(pin.tracker[i].ldr.day.down, INPUT);
    pinMode(pin.tracker[i].ldr.night, INPUT);
  }
  pinMode(pin.button.deploy, INPUT_PULLUP);
  pinMode(pin.button.retract, INPUT_PULLUP);
  pinMode(pin.button.automatic, INPUT_PULLUP);
  pinMode(pin.button.scan, INPUT_PULLUP);
  pinMode(pin.ledStatus, OUTPUT);
}

void Setup::blinkStatusLED(uint8_t pin) {
  for (uint8_t i = 0; i < 3; i++) {
    digitalWrite(pin, HIGH);
    delay(1000);
    digitalWrite(pin, LOW);
    delay(1000);
  }
}
