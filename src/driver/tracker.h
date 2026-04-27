#ifndef TRACKER_H
#define TRACKER_H
#include <Arduino.h>
#include "settings.h"
#include "ldr.h"
#include "motor.h"

class Tracker {
public:
  Tracker();
  Tracker(SettingsBoardPinTracker* trackerPin, int adcResolution, int pwmResolution, int motorSpeedPercent, uint16_t ldrThreshold);
  void init();
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
  uint16_t _ldrThreshold;
  SettingsBoardPinTracker *_trackerPin;
  LDRs _ldrs;
  Motors _motors;
};

#endif