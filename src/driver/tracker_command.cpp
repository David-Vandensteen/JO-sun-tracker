#include <Arduino.h>
#include <ADebouncer.h>
#include "tracker_command.h"

TrackerCommand::TrackerCommand(SettingBoardPinTrackerCommand *settingBoardPinTrackerCommand, SettingProgramTrackersCommand *settingProgramTrackersCommand)
  : _settingBoardPinTrackerCommand(settingBoardPinTrackerCommand), _settingProgramTrackersCommand(settingProgramTrackersCommand) {
  pinMode(_settingBoardPinTrackerCommand->deploy, INPUT_PULLUP);
  pinMode(_settingBoardPinTrackerCommand->retract, INPUT_PULLUP);
}

void TrackerCommand::init() {
  _deployButton->mode(DELAYED, _settingProgramTrackersCommand->debounce, HIGH);
  _retractButton->mode(DELAYED, _settingProgramTrackersCommand->debounce, HIGH);
}

void TrackerCommand::update() {
  bool deployPressed = _deployButton->debounce(digitalRead(_settingBoardPinTrackerCommand->deploy));
  bool retractPressed = _retractButton->debounce(digitalRead(_settingBoardPinTrackerCommand->retract));
}