#include <Arduino.h>
#include <ADebouncer.h>
#include "tracker_command.h"

// Public
TrackerCommand::TrackerCommand(SettingBoardPinTrackerCommand *settingBoardPinTrackerCommand, SettingProgramTrackersCommand *settingProgramTrackersCommand)
  : _settingBoardPinTrackerCommand(settingBoardPinTrackerCommand), _settingProgramTrackersCommand(settingProgramTrackersCommand) {
  _commandState = TrackerCommandState();
}

// Buttons use INPUT_PULLUP: pressed reads LOW, so the debounced output starts HIGH (released)
void TrackerCommand::init() {
  pinMode(_settingBoardPinTrackerCommand->deploy, INPUT_PULLUP);
  pinMode(_settingBoardPinTrackerCommand->retract, INPUT_PULLUP);
  _deployButton.mode(DELAYED, _settingProgramTrackersCommand->debounce, HIGH);
  _retractButton.mode(DELAYED, _settingProgramTrackersCommand->debounce, HIGH);
}

void TrackerCommand::update() {
  bool deployPressed = !_deployButton.debounce(digitalRead(_settingBoardPinTrackerCommand->deploy));
  bool retractPressed = !_retractButton.debounce(digitalRead(_settingBoardPinTrackerCommand->retract));

  if (deployPressed) {
    _commandState.setDeploy();
  } else if (retractPressed) {
    _commandState.setRetract();
  } else {
    _commandState.setIdle();
  }
}

TrackerCommandState TrackerCommand::state() {
  return _commandState;
}