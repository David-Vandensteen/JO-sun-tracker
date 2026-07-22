#ifndef TRACKER_H
#define TRACKER_H
#include <Arduino.h>
#include "command.h"
#include "ldrs.h"
#include "motors.h"
#include "setting.h"

class Tracker {
public:
  explicit Tracker(
    SettingBoardPinTracker* trackerPin,
    SettingBoardPinTrackerCommand* commandPin,
    SettingProgramLDR ldrSetting,
    uint16_t adcResolution,
    uint16_t pwmResolution,
    uint8_t motorSpeedPercent
  );
  void init();
  void deploy();
  void retract();
  void update();
  void stop();

private:
  enum class State {
    Idle,
    Deploying,
    Retracting,
  };
  SettingBoardPinTracker *_trackerPin;
  Ldrs _ldrs;
  Motors _motors;
  State _state = State::Idle;
  SettingProgramLDR _ldrSetting;
  uint16_t _pwmResolution;
  uint16_t _adcResolution;
  uint8_t _motorSpeedPercent;
};

#endif