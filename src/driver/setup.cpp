#include <Arduino.h>
#include "setup.h"

void Setup::pin(SettingsBoardPin pin) {
  pinMode(pin.ldr.day.up, INPUT);
  pinMode(pin.ldr.day.down, INPUT);
  pinMode(pin.ldr.night, INPUT);
  pinMode(pin.button.deploy, INPUT_PULLUP);
  pinMode(pin.button.retract, INPUT_PULLUP);
  pinMode(pin.button.automatic, INPUT_PULLUP);
  pinMode(pin.button.scan, INPUT_PULLUP);
  pinMode(pin.ledStatus, OUTPUT);
}

void Setup::blinkStatusLED(uint8_t pin) {
  for (int i = 0; i < 3; i++) {
    digitalWrite(pin, HIGH);
    delay(1000);
    digitalWrite(pin, LOW);
    delay(1000);
  }
}
