#include <Arduino.h>
#include <ArduinoLog.h>
#include "command.h"
#include "setting.h"
#include "tracker.h"
#include "ldr.h"
#include "motor.h"

Tracker::Tracker(
  SettingBoardPinTracker* trackerPin,
  SettingBoardPinTrackerCommand* commandPin,
  unsigned long samplingInterval,
  uint16_t adcResolution,
  uint16_t pwmResolution,
  uint16_t ldrThreshold,
  uint8_t motorSpeedPercent
)
  : _trackerPin(trackerPin),
    _adcResolution(adcResolution),
    _pwmResolution(pwmResolution),
    _motorSpeedPercent(motorSpeedPercent),
    _ldrThreshold(ldrThreshold),
    _ldrs(
      Ldr(trackerPin->ldr.up, adcResolution, samplingInterval),
      Ldr(trackerPin->ldr.down, adcResolution, samplingInterval)
    ),
    _motors(
      Motor(trackerPin->motors.in1, trackerPin->motors.in2, trackerPin->motors.ena, pwmResolution),
      Motor(trackerPin->motors.in3, trackerPin->motors.in4, trackerPin->motors.enb, pwmResolution)
    )
{};

void Tracker::init() {
  Log.traceln("Tracker::init");
  _state = State::Idle;
  _ldrs.init();
  _motors.init();
}

void Tracker::deploy() {
  Log.traceln("Tracker::deploy");
  _state = State::Deploying;
  _motors.deploy(_motorSpeedPercent);
}

void Tracker::retract() {
  Log.traceln("Tracker::retract");
  _state = State::Retracting;
  _motors.retract(_motorSpeedPercent);
}

void Tracker::stop() {
  Log.traceln("Tracker::stop");
  _state = State::Idle;
  _motors.stop();
}

void Tracker::update() {
  _ldrs.update();
}
