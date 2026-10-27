#ifndef TRACKER_COMMAND_H
#define TRACKER_COMMAND_H
#include <Arduino.h>
#include <ADebouncer.h>
#include "setting.h"
#include "tracker_command_state.h"

class TrackerCommand {
  public:
    explicit TrackerCommand(SettingBoardPinTrackerCommand *settingBoardPinTrackerCommand, SettingProgramTrackersCommand *settingProgramTrackersCommand);
    void init();
    void update();
    TrackerCommandState state();

  private:
    SettingBoardPinTrackerCommand *_settingBoardPinTrackerCommand;
    SettingProgramTrackersCommand *_settingProgramTrackersCommand;
    TrackerCommandState _commandState;
    ADebouncer _deployButton;
    ADebouncer _retractButton;
};


#endif // TRACKER_COMMAND_H