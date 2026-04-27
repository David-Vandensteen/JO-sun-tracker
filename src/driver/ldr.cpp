#include <Arduino.h>

#include "ldr.h"
#include "setting.h"

LDR::LDR(uint8_t pin, int analogResolution)
  : _pin(pin),
    _analogResolution(analogResolution),
    raw(0),
    percent(0)
{}

void LDR::init() {
  if (DEBUG) { Serial.print("LDR::init "); Serial.println(_pin); }
  pinMode(_pin, INPUT);
}

void LDR::update() {
  raw = analogRead(_pin);
  percent = map(raw, 0, _analogResolution, 100, 0);
}

LDRs::LDRs(LDR dayUp, LDR dayDown, LDR night)
  : dayUp(dayUp),
    dayDown(dayDown),
    night(night)
{}

void LDRs::init() {
  if (DEBUG) Serial.println("LDRs::init");
  dayUp.init();
  dayDown.init();
  night.init();
}

void LDRs::update() {
  dayUp.update();
  dayDown.update();
  night.update();
}

bool LDRs::isDayUpDifferentFromDayDown(long threshold) {
  return abs(dayUp.percent - dayDown.percent) > threshold;
}

bool LDRs::isDayUpBrighterThanDayDown(long threshold) {
  return dayUp.percent > dayDown.percent && isDayUpDifferentFromDayDown(threshold);
}

bool LDRs::isDayDownBrighterThanDayUp(long threshold) {
  return dayDown.percent > dayUp.percent && isDayUpDifferentFromDayDown(threshold);
}

