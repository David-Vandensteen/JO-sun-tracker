#include <Arduino.h>
#include <ArduinoLog.h>
#include <dv_iir_filter8.h>
#include "ldr.h"
#include "setting.h"

Ldr::Ldr(uint8_t pin, uint16_t adcResolution, unsigned long _samplingInterval)
  : _pin(pin),
    _adcResolution(adcResolution),
    _filter(0xFF),
    _samplingInterval(_samplingInterval),
    _lastUpdateTime(0)
{}

void Ldr::init() {
  Log.traceln("Ldr::init");
  pinMode(_pin, INPUT);
  _filter.reset();
}

void Ldr::update(unsigned long now) {
  if (now - _lastUpdateTime < _samplingInterval) { return; }

  _lastUpdateTime = now;
  uint16_t rawInput = analogRead(_pin);
  uint8_t inputPercent = (uint8_t)(((uint32_t)rawInput * 100U) / _adcResolution);
  uint8_t filteredPercent = _filter.update(inputPercent);

  if (LOG) {
    Serial.print("input-id"); Serial.print(_pin); Serial.print(":");  Serial.print(inputPercent);
    Serial.println("");

    Serial.print("filtered-id"); Serial.print(_pin); Serial.print(":"); Serial.print(filteredPercent);
    Serial.println("");
  }

}
