#include <Arduino.h>
#include <ArduinoLog.h>
#include "ldr.h"
#include "ldrs.h"

Ldrs::Ldrs(Ldr up, Ldr down)
  : up(up),
    down(down)
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

  if (upValue > downValue) {
    return ldrsComparison::UpGreaterThanDown;
  } else if (downValue > upValue) {
    return ldrsComparison::DownGreaterThanUp;
  } else {
    return ldrsComparison::Deadband;
  }
}
