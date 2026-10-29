#include <Arduino.h>
#include <ArduinoLog.h>
#include "ldr.h"
#include "ldrs.h"
#include "setting.h"

// Public
Ldrs::Ldrs(Ldr ldrUp, Ldr ldrDown, SettingProgramLDRs *settingProgramLDRs)
  : ldrUp(ldrUp),
    ldrDown(ldrDown),
    _settingProgramLDRs(settingProgramLDRs)
{}

void Ldrs::init() {
  Log.traceln("Ldrs::init");
  ldrUp.init();
  ldrDown.init();
}

uint8_t Ldrs::getUpAveragePercentValue() {
  return ldrUp.getAveragePercentValue();
}

uint8_t Ldrs::getDownAveragePercentValue() {
  return ldrDown.getAveragePercentValue();
}

LdrsComparison Ldrs::getComparison() {
  uint8_t upValue = ldrUp.getAveragePercentValue();
  uint8_t downValue = ldrDown.getAveragePercentValue();

  if (upValue <= _settingProgramLDRs->night.cutoff && downValue <= _settingProgramLDRs->night.cutoff) {
    return LdrsComparison::NIGHT;
  }

  const int16_t delta = (int16_t)upValue - (int16_t)downValue;
  if (abs(delta) <= _settingProgramLDRs->threshold) {
    return LdrsComparison::DEADBAND;
  }
  if (delta > 0) {
    return LdrsComparison::UP_GREATER_THAN_DOWN;
  }

  return LdrsComparison::DOWN_GREATER_THAN_UP;
}

LdrsComparison Ldrs::update() {
  ldrUp.update();
  ldrDown.update();

  return Ldrs::getComparison();
}
