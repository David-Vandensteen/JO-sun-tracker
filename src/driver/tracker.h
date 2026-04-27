#ifndef TRACKER_H
#define TRACKER_H
#include "settings.h"
#include "ldr.h"
#include "motor.h"

class Tracker {
public:
  Tracker();
  Tracker(SettingsBoardPinTracker *trackerPin, int adcResolution, int pwmResolution);
  void init();
  void deploy(int speedPercent);
  void retract(int speedPercent);
  void scan(int speedPercent);
  void update();

private:
  int _adcResolution;
  int _pwmResolution;
  SettingsBoardPinTracker *_trackerPin;
  LDRs _ldrs;
  Motors _motors;
};

#endif