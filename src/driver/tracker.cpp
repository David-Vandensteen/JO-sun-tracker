#include <Arduino.h>
#include <ArduinoLog.h>
#include "setting.h"
#include "tracker.h"
#include "ldr.h"
#include "motor.h"

Tracker::Tracker(SettingBoardPinTracker* trackerPin, int adcResolution, int pwmResolution, int motorSpeedPercent, uint16_t ldrThreshold)
  : _trackerPin(trackerPin),
    _adcResolution(adcResolution),
    _pwmResolution(pwmResolution),
    _motorSpeedPercent(motorSpeedPercent),
    _ldrThreshold(ldrThreshold),
    _ldrs(
      Ldr(trackerPin->ldr.up, adcResolution),
      Ldr(trackerPin->ldr.down, adcResolution)
    ),
    _motors(
      Motor(trackerPin->motors.in1, trackerPin->motors.in2, trackerPin->motors.ena, pwmResolution),
      Motor(trackerPin->motors.in3, trackerPin->motors.in4, trackerPin->motors.enb, pwmResolution)
    )
{}

void Tracker::init() {
  Log.traceln("Tracker::init");
  _ldrs.init();
  _motors.init();
}

bool Tracker::isAutoMode() {
  return _isAutoMode;
}

void Tracker::setAutoMode(bool autoMode) {
  _isAutoMode = autoMode;
  if (_isAutoMode) {
    Log.traceln("Tracker::setAutoMode auto mode");
  } else {
    Log.traceln("Tracker::setAutoMode manual mode");
  }
}

void Tracker::deploy() {
  Log.traceln("Tracker::deploy");
  _motors.deploy(_motorSpeedPercent);
}

void Tracker::retract() {
  Log.traceln("Tracker::retract");
  _motors.retract(_motorSpeedPercent);
}

void Tracker::scan() {
  Log.traceln("Tracker::scan");
  Log.noticeln("Scanning tracker");
  setAutoMode(true);
  // Implement scan functionality here
}

void Tracker::stop() {
  Log.traceln("Tracker::stop");
  _motors.stop();
}

void Tracker::updateAutoMode() {
  unsigned long now = millis();
  unsigned long lastIterationTime = 0;
  if (now - lastIterationTime <= 1000) {
    _ldrs.update();
    bool isLDRDifferent = _ldrs.isDayUpDifferentFromDayDown(_ldrThreshold);

    if (isLDRDifferent) Log.traceln("LDR values are different");

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
