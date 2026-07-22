#ifndef TRACKER_H
#define TRACKER_H
#include <Arduino.h>
#include "command.h"
#include "ldrs.h"
#include "motor.h"

class Tracker {
public:
  explicit Tracker(
    SettingBoardPinTracker* trackerPin,
    SettingBoardPinTrackerCommand* commandPin,
    uint16_t adcResolution,
    uint16_t pwmResolution,
    uint16_t ldrThreshold,
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
  uint16_t _adcResolution;
  uint16_t _ldrThreshold;
  SettingBoardPinTracker *_trackerPin;
  Ldrs _ldrs;
  Motors _motors;
  uint8_t _pwmResolution;
  uint8_t _motorSpeedPercent;
  State _state = State::Idle;
};

#endif