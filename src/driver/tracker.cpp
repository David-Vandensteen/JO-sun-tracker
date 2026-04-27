#include "settings.h"
#include "tracker.h"
#include "ldr.h"
#include "motor.h"

Tracker::Tracker()
  : _trackerPin(nullptr), _adcResolution(0), _pwmResolution(0),
    _ldrs(LDR(0,0), LDR(0,0), LDR(0,0)),
    _motors(Motor(0,0,0,0), Motor(0,0,0,0))
{}

Tracker::Tracker(SettingsBoardPinTracker *trackerPin, int adcResolution, int pwmResolution)
  : _trackerPin(trackerPin),
    _adcResolution(adcResolution),
    _pwmResolution(pwmResolution),
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

void Tracker::deploy(int speedPercent) {
  if (DEBUG) { Serial.print("Tracker::deploy "); Serial.println(speedPercent); }
  _motors.deploy(speedPercent);
}

void Tracker::retract(int speedPercent) {
  if (DEBUG) { Serial.print("Tracker::retract "); Serial.println(speedPercent); }
  _motors.retract(speedPercent);
}

void Tracker::scan(int speedPercent) {
  if (DEBUG) { Serial.print("Tracker::scan "); Serial.println(speedPercent); }
  // Implement scan functionality here
}

void Tracker::updateAutoMode() {
  // Implement auto mode update logic here
}

void Tracker::updateManualMode() {
  // Implement manual mode update logic here
}

