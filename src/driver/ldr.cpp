#include <Arduino.h>

#include "ldr.h"
#include "setting.h"
#include <ArduinoLog.h>

Ldr::Ldr(uint8_t pin, int analogResolution)
  : _pin(pin),
    _analogResolution(analogResolution),
    raw(0),
    percent(0)
{}

void Ldr::init() {
  Log.trace("Ldr::init\n");
  pinMode(_pin, INPUT);
}

void Ldr::update() {
  raw = analogRead(_pin);
  percent = map(raw, 0, _analogResolution, 100, 0);
}

Ldrs::Ldrs(Ldr dayUp, Ldr dayDown)
  : dayUp(dayUp),
    dayDown(dayDown)
{}

void Ldrs::init() {
  Log.trace("Ldrs::init\n");
  dayUp.init();
  dayDown.init();
}

void Ldrs::update() {
  Log.trace("Ldrs::update\n");
  dayUp.update();
  dayDown.update();
}

bool Ldrs::isDayUpDifferentFromDayDown(long threshold) {
  return abs(dayUp.percent - dayDown.percent) > threshold;
}

bool Ldrs::isDayUpBrighterThanDayDown(long threshold) {
  return dayUp.percent > dayDown.percent && isDayUpDifferentFromDayDown(threshold);
}

bool Ldrs::isDayDownBrighterThanDayUp(long threshold) {
  return dayDown.percent > dayUp.percent && isDayUpDifferentFromDayDown(threshold);
}
