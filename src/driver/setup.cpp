#include <Arduino.h>
#include "setup.h"

static void Setup::pin(SettingsPin pin) {
  pinMode(pin.LDR.day.up, INPUT);
  pinMode(pin.LDR.day.down, INPUT);
  pinMode(pin.LDR.night, INPUT);
  pinMode(pin.button.deploy, INPUT_PULLUP);
  pinMode(pin.button.retract, INPUT_PULLUP);
  pinMode(pin.button.automatic, INPUT_PULLUP);
  pinMode(pin.button.scan, INPUT_PULLUP);
  pinMode(pin.LEDStatus, OUTPUT);
}

static void Setup::blinkStatusLED(uint8_t pin) {
  for (int i = 0; i < 3; i++) {
    digitalWrite(pin, HIGH);
    delay(1000);
    digitalWrite(pin, LOW);
    delay(1000);
  }
}
