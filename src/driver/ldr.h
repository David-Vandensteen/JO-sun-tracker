#ifndef LDR_H
#define LDR_H
#include <Arduino.h>
#include <dv_every_interval.h>
#include <dv_average_accumulator8.h>
#include "setting.h"

class Ldr {
public:
  explicit Ldr(uint8_t pin, uint16_t adcResolution, SettingProgramLDR *settingProgramLDR);
  uint8_t getAveragePercentValue();
  uint8_t update();
  void init();

private:
  static void onIntervalTick(void *ctx);
  DV_AverageAccumulator8 _averageAccumulator8;
  DV_EveryInterval _everyInterval;
  SettingProgramLDR *_settingProgramLDR;
  uint16_t _adcResolution;
  uint8_t _pin;
  uint8_t interval();
};

#endif