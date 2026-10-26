#ifndef TRACKER_H
#define TRACKER_H
#include <Arduino.h>
#include <dv_every_interval.h>
#include "ldrs.h"
#include "motors.h"
#include "setting.h"
#include "tracker_command.h"
#include "tracker_state.h"

class Tracker {
public:
  explicit Tracker(
    SettingBoardPinTracker *settingBoardPinTracker,
    SettingBoardPinMode *settingBoardPinMode,
    SettingBoardPinDriverCommand *settingBoardPinDriverCommand,
    SettingProgramTrackers *settingProgramTrackers,
    SettingProgramLDR *settingProgramLDR,
    SettingProgramLDRs *settingProgramLDRs,
    SettingBoardADC *settingBoardADC,
    SettingBoardPWM *settingBoardPWM,
    SettingProgramMotors *settingProgramMotors
  );
  void init();
  void update();
  void stop();

private:
  static void onIntervalTick(void *ctx);
  SettingBoardPinMode *_settingBoardPinMode;
  Ldrs _ldrs;
  Motors _motors;
  TrackerCommand _trackerCommand;
  TrackerCommandState _commandState;
  SettingProgramMotors *_settingProgramMotors;
  SettingProgramTrackers *_settingProgramTrackers;
  DV_EveryInterval _everyInterval;
  TrackerState _state;
  bool isManualMode();
  void deploy();
  void retract();
  void handleCommand();
  void interval();
};

#endif