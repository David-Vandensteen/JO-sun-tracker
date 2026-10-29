#include <Arduino.h>
#include <ADebouncer.h>
#include "tracker_command.h"

// Public
TrackerCommand::TrackerCommand(SettingBoardPinTrackerCommand *settingBoardPinTrackerCommand, SettingProgramTrackerCommand *settingProgramTrackerCommand)
  : _settingBoardPinTrackerCommand(settingBoardPinTrackerCommand), _settingProgramTrackerCommand(settingProgramTrackerCommand) {
  _commandState = TrackerCommandState();
}

// Buttons use INPUT_PULLUP: pressed reads LOW, so the debounced output starts HIGH (released)
void TrackerCommand::init() {
  pinMode(_settingBoardPinTrackerCommand->mode, INPUT_PULLUP);
  pinMode(_settingBoardPinTrackerCommand->deploy, INPUT_PULLUP);
  pinMode(_settingBoardPinTrackerCommand->retract, INPUT_PULLUP);
  _deployButton.mode(DELAYED, _settingProgramTrackerCommand->debounce, HIGH);
  _retractButton.mode(DELAYED, _settingProgramTrackerCommand->debounce, HIGH);
  // Switch pulled up: LOW selects manual, so the debounced output starts HIGH (automatic) when released
  _modeSwitch.mode(DELAYED, _settingProgramTrackerCommand->debounce, digitalRead(_settingBoardPinTrackerCommand->mode));
  if (_modeSwitch.debounced() == HIGH) {
    _commandState.setAuto();
  }
}

void TrackerCommand::update() {
  // Two-position switch: LOW selects manual mode, buttons are only read in manual mode
  bool manualMode = !_modeSwitch.debounce(digitalRead(_settingBoardPinTrackerCommand->mode));
  if (!manualMode) {
    _bothPressed = false;
    _commandState.setAuto();
    return;
  }

  bool deployPressed = !_deployButton.debounce(digitalRead(_settingBoardPinTrackerCommand->deploy));
  bool retractPressed = !_retractButton.debounce(digitalRead(_settingBoardPinTrackerCommand->retract));

  if (deployPressed && retractPressed) {
    // Ambiguous input: both buttons pressed, so stop until a single command is detected
    if (!_bothPressed) {
      _bothPressed = true;
      _bothPressedSinceMs = millis();
    }
    if (millis() - _bothPressedSinceMs >= _settingProgramTrackerCommand->resetHoldMs) {
      _commandState.setReset();
    } else {
      _commandState.setIdle();
    }
    return;
  }
  _bothPressed = false;

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