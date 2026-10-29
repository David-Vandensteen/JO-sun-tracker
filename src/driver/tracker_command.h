#ifndef TRACKER_COMMAND_H
#define TRACKER_COMMAND_H
#include <Arduino.h>
#include <ADebouncer.h>
#include "setting.h"
#include "tracker_command_state.h"

class TrackerCommand {
  public:
    explicit TrackerCommand(SettingBoardPinTrackerCommand *settingBoardPinTrackerCommand, SettingProgramTrackerCommand *settingProgramTrackerCommand);
    void init();
    void update();
    TrackerCommandState state();

  private:
    SettingBoardPinTrackerCommand *_settingBoardPinTrackerCommand;
    SettingProgramTrackerCommand *_settingProgramTrackerCommand;
    TrackerCommandState _commandState;
    ADebouncer _deployButton;
    ADebouncer _retractButton;
    ADebouncer _modeSwitch;
    bool _bothPressed = false;
    unsigned long _bothPressedSinceMs = 0;
};


#endif // TRACKER_COMMAND_H