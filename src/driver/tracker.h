#ifndef TRACKER_H
#define TRACKER_H
#include <Arduino.h>
#include <dv_every_interval.h>
#include "commands.h"
#include "ldrs.h"
#include "motors.h"
#include "setting.h"
#include "state.h"
#include "command_state.h"

class Tracker {
public:
  explicit Tracker(
    SettingBoardPinTracker *settingBoardPinTracker,
    SettingBoardPinMode *settingBoardPinMode,
    SettingProgramTrackers *settingProgramTrackers,
    SettingProgramLDR *settingProgramLDR,
    SettingProgramLDRs *settingProgramLDRs,
    SettingBoardADC *settingBoardADC,
    SettingBoardPWM *settingBoardPWM,
    SettingProgramMotors *settingProgramMotors
  );
  void init();
  State update();

private:
  static void onIntervalTick(void *ctx);
  SettingBoardPinMode *_settingBoardPinMode;
  Ldrs _ldrs;
  Motors _motors;
  SettingProgramMotors *_settingProgramMotors;
  SettingProgramTrackers *_settingProgramTrackers;
  Commands _command;
  DV_EveryInterval _everyInterval;
  State _state;
  bool isManualMode();
  void deploy();
  void retract();
  void stop();
  void interval();
};

#endif