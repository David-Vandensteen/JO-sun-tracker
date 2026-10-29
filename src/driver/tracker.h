#ifndef TRACKER_H
#define TRACKER_H
#include <Arduino.h>
#include <dv_every_interval.h>
#include "ldrs.h"
#include "motors.h"
#include "setting.h"
#include "app_status.h"
#include "tracker_command.h"

class Tracker {
public:
  explicit Tracker(
    SettingBoardPinTracker *settingBoardPinTracker,
    SettingProgramTracker *settingProgramTracker,
    SettingProgramLDR *settingProgramLDR,
    SettingProgramLDRs *settingProgramLDRs,
    SettingBoardADC *settingBoardADC,
    SettingBoardPWM *settingBoardPWM,
    SettingProgramMotors *settingProgramMotors
  );
  void init();
  void update();
  void stop();
  AppTrackerStatus AppStatus();
  bool isResetRequested();

private:
  static void onIntervalTick(void *ctx);
  Ldrs _ldrs;
  Motors _motors;
  TrackerCommand _trackerCommand;
  TrackerCommandState _commandState;
  SettingProgramTracker *_settingProgramTracker;
  DV_EveryInterval _everyInterval;
  unsigned long _nextComparisonMs = 0;
  void deploy();
  void retract();
  void handleCommand();
  void interval();
};

#endif