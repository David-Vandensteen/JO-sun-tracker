#include <Arduino.h>
#include <ArduinoLog.h>
#include <dv_iir_filter8.h>
#include "ldr.h"
#include "setting.h"

DV_IirFilter8 filter(0xFF);

Ldr::Ldr(uint8_t pin, uint16_t adcResolution)
  : _pin(pin),
    _adcResolution(adcResolution),
    raw(0),
    percent(0)
{}

void Ldr::init() {
  Log.traceln("Ldr::init");
  pinMode(_pin, INPUT);
}

void Ldr::update() {
  uint16_t rawInput = analogRead(_pin);
  uint8_t inputPercent = (uint8_t)(((uint32_t)rawInput * 100U) / _adcResolution);
  uint8_t filteredPercent = filter.update(inputPercent);

  Serial.print("input:");
  Serial.print(inputPercent);
  Serial.print(" raw:");
  Serial.print(rawInput);
  Serial.print(" filtered:");
  Serial.println(filteredPercent);
}
