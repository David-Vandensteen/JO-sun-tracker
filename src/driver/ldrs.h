#ifndef LDRS_H
#define LDRS_H
#include "ldr.h"
#include "setting.h"

enum class LdrsComparison {
  UP_GREATER_THAN_DOWN,
  DOWN_GREATER_THAN_UP,
  DEADBAND,
  NIGHT
};

class Ldrs {
public:
  explicit Ldrs(Ldr ldrUp, Ldr ldrDown, SettingProgramLDRs *settingProgramLDRs);
  uint8_t getUpAveragePercentValue();
  uint8_t getDownAveragePercentValue();
  LdrsComparison getComparison();
  LdrsComparison update();
  void init();

private:
  Ldr ldrUp;
  Ldr ldrDown;
  SettingProgramLDRs *_settingProgramLDRs;
};

#endif