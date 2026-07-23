#ifndef LDR_H
#define LDR_H
#include <Arduino.h>
#include <dv_iir_filter8.h>
#include "setting.h"

class Ldr {
public:
  int raw;
  explicit Ldr(uint8_t pin, uint16_t adcResolution, SettingProgramLDR ldrSetting);
  void init();
  uint8_t update(unsigned long now);

private:
  DV_IirFilter8 _filter;
  unsigned long _lastUpdateTime;
  unsigned long _samplingInterval;
  uint16_t _adcResolution;
  uint8_t _pin;
};

#endif