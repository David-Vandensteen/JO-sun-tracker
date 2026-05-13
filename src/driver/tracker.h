#ifndef TRACKER_H
#define TRACKER_H
#include <Arduino.h>
#include "ldr.h"
#include "motor.h"

class Tracker {
public:
  explicit Tracker(SettingBoardPinTracker* trackerPin, int adcResolution, int pwmResolution, int motorSpeedPercent, uint16_t ldrThreshold);
  void init();
  bool isAutoMode();
  void setAutoMode(bool autoMode);
  void deploy();
  void retract();
  void scan();
  void updateAutoMode();
  void stop();

private:
  enum class State {
    Idle,
    Deploying,
    Retracting,
    Auto
  };
  int _adcResolution;
  int _pwmResolution;
  int _motorSpeedPercent;
  bool _isAutoMode = true;
  uint16_t _ldrThreshold;
  SettingBoardPinTracker *_trackerPin;
  Ldrs _ldrs;
  Motors _motors;
  State _state = State::Idle;
};

#endif