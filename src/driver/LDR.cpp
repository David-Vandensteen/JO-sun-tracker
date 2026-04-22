#include <Arduino.h>
#include "LDR.h"
#include "settings.h"

LDR::LDR(uint8_t pin, int analogResolution)
  : _pin(pin),
    _analogResolution(analogResolution),
    raw(0),
    percent(0)
{}

void LDR::read() {
  raw = analogRead(_pin);
  percent = map(raw, 0, _analogResolution, 100, 0);
}

LDRs::LDRs(LDR dayUp, LDR dayDown, LDR night)
  : dayUp(dayUp),
    dayDown(dayDown),
    night(night)
{}

void LDRs::read() {
  dayUp.read();
  dayDown.read();
  night.read();
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

