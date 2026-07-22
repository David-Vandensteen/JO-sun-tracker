#include <Arduino.h>
#include <ArduinoLog.h>
#include "ldr.h"
#include "setting.h"

Ldr::Ldr(uint8_t pin, uint16_t adcResolution)
  : _pin(pin),
    _adcResolution(adcResolution),
    raw(0),
    percent(0)
{}

void Ldr::init() {
  Log.traceln("Ldr::init");
  pinMode(_pin, INPUT);
}

void Ldr::update() {
  raw = analogRead(_pin);
  percent = map(raw, 0, _adcResolution, 100, 0);
}

Ldrs::Ldrs(Ldr dayUp, Ldr dayDown)
  : dayUp(dayUp),
    dayDown(dayDown)
{}

void Ldrs::init() {
  Log.traceln("Ldrs::init");
  dayUp.init();
  dayDown.init();
}

void Ldrs::update() {
  Log.traceln("Ldrs::update");
  dayUp.update();
  dayDown.update();
}
