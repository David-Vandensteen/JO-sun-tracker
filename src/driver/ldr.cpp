#include <Arduino.h>
#include <ArduinoLog.h>
#include <dv_every_interval.h>
#include <dv_average_accumulator8.h>
#include "ldr.h"
#include "setting.h"

// Public
Ldr::Ldr(uint8_t pin, uint16_t adcResolution, SettingProgramLDR *settingProgramLDR)
  : _pin(pin),
    _adcResolution(adcResolution),
    _settingProgramLDR(settingProgramLDR) {}

void Ldr::init() {
  Log.traceln("Ldr::init - pin: %d, sampling interval: %lu", _pin, _settingProgramLDR->sampling.interval);
  #if MOCK_LDR_AVERAGE_PERCENT_VALUE
  Log.traceln("Ldr::init - MOCK AveragePercentValue");
  #endif
  pinMode(_pin, INPUT);
  _everyInterval
    .setInterval(_settingProgramLDR->sampling.interval)
    .setCallback(Ldr::onIntervalTick, this);
}

uint8_t Ldr::getAveragePercentValue() {
  // mock
  #if MOCK_LDR_AVERAGE_PERCENT_VALUE
  if (_pin == getSetting()->board.pin.tracker.ldr.up) {
    return uint8_t(30);
  }
  if (_pin == getSetting()->board.pin.tracker.ldr.down) {
    return uint8_t(10);
  }
  #endif
  //

  return _averageAccumulator8.getValue();
}

uint8_t Ldr::update() {
  _everyInterval.update();

  return _averageAccumulator8.getValue();
}

// Private
void Ldr::onIntervalTick(void *ctx) {
  if (ctx == nullptr) {
    return;
  }

  Ldr *self = static_cast<Ldr *>(ctx);
  self->interval();
}

uint8_t Ldr::interval() {
  uint16_t rawInput = analogRead(_pin);
  uint8_t inputPercent = (uint8_t)(((uint32_t)rawInput * 100U) / _adcResolution);
  _averageAccumulator8.add(inputPercent);
  #if LOG_LDR_INTERVAL
  Log.noticeln("ldr-%d:%d, ldr-%d-average:%d", _pin, inputPercent, _pin, getAveragePercentValue());
  #endif
  return _averageAccumulator8.getValue();
}
