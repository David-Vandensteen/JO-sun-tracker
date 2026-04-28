#include <Arduino.h>
#include "setting.h"
#include "tracker.h"
#include "ldr.h"
#include "motor.h"

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
  if (DEBUG) Serial.println("Tracker::init");
  _ldrs.init();
  _motors.init();
}

void Tracker::deploy() {
  if (DEBUG) { Serial.print("Tracker::deploy "); Serial.println(_motorSpeedPercent); }
  _motors.deploy(_motorSpeedPercent);
}

void Tracker::retract() {
  if (DEBUG) { Serial.print("Tracker::retract "); Serial.println(_motorSpeedPercent); }
  _motors.retract(_motorSpeedPercent);
}

void Tracker::scan() {
  if (DEBUG) { Serial.print("Tracker::scan "); Serial.println(_motorSpeedPercent); }
  // Implement scan functionality here
}

void Tracker::stop() {
  _motors.stop();
}

void Tracker::updateAutoMode() {
  unsigned long now = millis();
  unsigned long lastIterationTime = 0;
  if (now - lastIterationTime <= 1000) {
    _ldrs.update();
    bool isLDRDifferent = _ldrs.isDayUpDifferentFromDayDown(_ldrThreshold);

    if (DEBUG && isLDRDifferent) Serial.println("LDR values are different");

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

void Tracker::updateManualMode(bool deployButton, bool retractButton) {
  if (deployButton && retractButton) {
    if (DEBUG) Serial.println("Tracker::updateManualMode both deploy and retract buttons pressed, stopping");
    stop();
    return;
  }
  if (deployButton) {
    if (DEBUG) Serial.println("Tracker::updateManualMode deploy button pressed");
    deploy();
    return;
  }
  if (retractButton) {
    if (DEBUG) Serial.println("Tracker::updateManualMode retract button pressed");
    retract();
    return;
  }
  stop();
}
