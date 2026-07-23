#include <Arduino.h>
#include <ArduinoLog.h>
#include "ldr.h"
#include "ldrs.h"

Ldrs::Ldrs(Ldr up, Ldr down, uint8_t threshold)
  : up(up),
    down(down),
    _threshold(threshold)
{}

void Ldrs::init() {
  Log.traceln("Ldrs::init");
  up.init();
  down.init();
}

ldrsComparison Ldrs::update() {
  if (
    millis() - up.getLastUpdateTime() < up.getSamplingInterval()
    || millis() - down.getLastUpdateTime() < down.getSamplingInterval()
  ) {
    return ldrsComparison::NotUpdated;
  }

  uint8_t upValue = up.update();
  uint8_t downValue = down.update();

  const int16_t delta = (int16_t)upValue - (int16_t)downValue;
  if (abs(delta) <= _threshold) {
    return ldrsComparison::Deadband;
  }
  if (delta > 0) {
    return ldrsComparison::UpGreaterThanDown;
  }

  return ldrsComparison::DownGreaterThanUp;
}
