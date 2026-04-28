#include <Arduino.h>

#include "setting.h"
#include "tracker.h"
#include "ldr.h"
#include "motor.h"
#include "log.h"

Tracker::Tracker()
  : _trackerPin(nullptr), _adcResolution(0), _pwmResolution(0), _motorSpeedPercent(0), _ldrThreshold(0),
    _ldrs(LDR(0,0), LDR(0,0), LDR(0,0)),
    _motors(Motor(0,0,0,0), Motor(0,0,0,0))
{}

Tracker::Tracker(SettingBoardPinTracker* trackerPin, int adcResolution, int pwmResolution, int motorSpeedPercent, uint16_t ldrThreshold)
  : _trackerPin(trackerPin),
    _adcResolution(adcResolution),
    _pwmResolution(pwmResolution),
    _motorSpeedPercent(motorSpeedPercent),
    _ldrThreshold(ldrThreshold),
    _ldrs(
      LDR(trackerPin->ldr.day.up, adcResolution),
      LDR(trackerPin->ldr.day.down, adcResolution),
      LDR(trackerPin->ldr.night, adcResolution)
    ),
    _motors(
      Motor(trackerPin->motors.in1, trackerPin->motors.in2, trackerPin->motors.ena, pwmResolution),
      Motor(trackerPin->motors.in3, trackerPin->motors.in4, trackerPin->motors.enb, pwmResolution)
    )
{}

void Tracker::init() {
  LOG_DEBUG("Tracker::init");
  _ldrs.init();
  _motors.init();
}

bool Tracker::isAutoMode() {
  return _isAutoMode;
}

void Tracker::setAutoMode(bool autoMode) {
  _isAutoMode = autoMode;
  if (_isAutoMode) {
    LOG_DEBUG("Tracker::setAutoMode auto mode");
  } else {
    LOG_DEBUG("Tracker::setAutoMode manual mode");
  }
}

void Tracker::deploy() {
  LOG_DEBUG("Tracker::deploy");
  _motors.deploy(_motorSpeedPercent);
}

void Tracker::retract() {
  LOG_DEBUG("Tracker::retract");
  _motors.retract(_motorSpeedPercent);
}

void Tracker::scan() {
  LOG_DEBUG("Tracker::scan");
  setAutoMode(true);
  // Implement scan functionality here
}

void Tracker::stop() {
  LOG_DEBUG("Tracker::stop");
  _motors.stop();
}

void Tracker::updateAutoMode() {
  unsigned long now = millis();
  unsigned long lastIterationTime = 0;
  if (now - lastIterationTime <= 1000) {
    _ldrs.update();
    bool isLDRDifferent = _ldrs.isDayUpDifferentFromDayDown(_ldrThreshold);

    if (isLDRDifferent) LOG_DEBUG("LDR values are different");

    if (isLDRDifferent) {
      if (_ldrs.isDayUpBrighterThanDayDown(_ldrThreshold)) {
        deploy();
      } else if (_ldrs.isDayDownBrighterThanDayUp(_ldrThreshold)) {
        retract();
      }
    } else {
      stop();
    }
  }
}
