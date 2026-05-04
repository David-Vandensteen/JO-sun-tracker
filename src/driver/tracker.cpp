#include <Arduino.h>

#include "setting.h"
#include "tracker.h"
#include "ldr.h"
#include "motor.h"
#include <ArduinoLog.h>

Tracker::Tracker(SettingBoardPinTracker* trackerPin, int adcResolution, int pwmResolution, int motorSpeedPercent, uint16_t ldrThreshold)
  : _trackerPin(trackerPin),
    _adcResolution(adcResolution),
    _pwmResolution(pwmResolution),
    _motorSpeedPercent(motorSpeedPercent),
    _ldrThreshold(ldrThreshold),
    _ldrs(
      LDR(trackerPin->ldr.day.up, adcResolution),
      LDR(trackerPin->ldr.day.down, adcResolution),
      LDR(trackerPin->ldr.back, adcResolution)
    ),
    _motors(
      Motor(trackerPin->motors.in1, trackerPin->motors.in2, trackerPin->motors.ena, pwmResolution),
      Motor(trackerPin->motors.in3, trackerPin->motors.in4, trackerPin->motors.enb, pwmResolution)
    )
{}

void Tracker::init() {
  Log.trace("Tracker::init\n");
  _ldrs.init();
  _motors.init();
}

bool Tracker::isAutoMode() {
  return _isAutoMode;
}

void Tracker::setAutoMode(bool autoMode) {
  _isAutoMode = autoMode;
  if (_isAutoMode) {
    Log.trace("Tracker::setAutoMode auto mode\n");
  } else {
    Log.trace("Tracker::setAutoMode manual mode\n");
  }
}

void Tracker::deploy() {
  Log.trace("Tracker::deploy\n");
  _motors.deploy(_motorSpeedPercent);
}

void Tracker::retract() {
  Log.trace("Tracker::retract\n");
  _motors.retract(_motorSpeedPercent);
}

void Tracker::scan() {
  Log.trace("Tracker::scan\n");
  Log.notice("Scanning tracker\n");
  setAutoMode(true);
  // Implement scan functionality here
}

void Tracker::stop() {
  Log.trace("Tracker::stop\n");
  _motors.stop();
}

void Tracker::updateAutoMode() {
  unsigned long now = millis();
  unsigned long lastIterationTime = 0;
  if (now - lastIterationTime <= 1000) {
    _ldrs.update();
    bool isLDRDifferent = _ldrs.isDayUpDifferentFromDayDown(_ldrThreshold);

    if (isLDRDifferent) Log.trace("LDR values are different\n");

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
