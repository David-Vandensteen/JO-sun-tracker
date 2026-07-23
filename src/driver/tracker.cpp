#include <Arduino.h>
#include <ArduinoLog.h>
#include "command.h"
#include "setting.h"
#include "tracker.h"
#include "ldr.h"
#include "ldrs.h"
#include "motor.h"

Tracker::Tracker(
  SettingBoardPinTracker *trackerPin,
  SettingBoardPinTrackerCommand *commandPin,
  SettingBoardPinMode *modePin,
  SettingProgramLDR *ldrSetting,
  uint16_t adcResolution,
  uint16_t pwmResolution,
  uint8_t motorSpeedPercent
)
  : _motorSpeedPercent(motorSpeedPercent),
    _modePin(modePin),
    _ldrs(
      Ldr(
        trackerPin->ldr.up,
        adcResolution,
        ldrSetting
      ),
      Ldr(
        trackerPin->ldr.down,
        adcResolution,
        ldrSetting
      )
    ),
    _motors(
      Motor(
        trackerPin->motors.in1,
        trackerPin->motors.in2,
        trackerPin->motors.ena,
        pwmResolution
      ),
      Motor(
        trackerPin->motors.in3,
        trackerPin->motors.in4,
        trackerPin->motors.enb,
        pwmResolution
      )
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
  ldrsComparison comparison = _ldrs.update();
  switch (comparison)
  {
  case ldrsComparison::UpGreaterThanDown:
    Log.traceln("Tracker::update - LDR UpGreaterThanDown");
    deploy();
    break;
  case ldrsComparison::DownGreaterThanUp:
    Log.traceln("Tracker::update - LDR DownGreaterThanUp");
    retract();
    break;
  case ldrsComparison::Deadband:
    Log.traceln("Tracker::update - LDR Deadband");
    break;
  default:
    break;
  }
}
