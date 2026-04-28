#ifndef TRACKER_H
#define TRACKER_H
#include <Arduino.h>

#include "ldr.h"
#include "motor.h"

class Tracker {
public:
  Tracker();
    Tracker(SettingBoardPinTracker* trackerPin, int adcResolution, int pwmResolution, int motorSpeedPercent, uint16_t ldrThreshold);
  void init();
  bool isAutoMode();
  void setAutoMode(bool autoMode);
  void deploy();
  void retract();
  void scan();
  void stop();
  void updateAutoMode();
  void updateManualMode(bool deployButton, bool retractButton);

private:
  int _adcResolution;
  int _pwmResolution;
  int _motorSpeedPercent;
  bool _isAutoMode = true;
  uint16_t _ldrThreshold;
  SettingBoardPinTracker *_trackerPin;
  LDRs _ldrs;
  Motors _motors;
};

#endif