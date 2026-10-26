#include <Arduino.h>
#include <ADebouncer.h>
#include "tracker_command.h"

TrackerCommand::TrackerCommand(SettingBoardPinTrackerCommand *pinCommand) {
  _pinCommand = pinCommand;
  pinMode(_pinCommand->deploy, INPUT_PULLUP);
  pinMode(_pinCommand->retract, INPUT_PULLUP);
}

void TrackerCommand::init() {
  _deployButton->mode(DELAYED, 10, HIGH);
  _retractButton->mode(DELAYED, 10, HIGH);
}

void TrackerCommand::update() {
  bool deployPressed = _deployButton->debounce(digitalRead(_pinCommand->deploy));
  bool retractPressed = _retractButton->debounce(digitalRead(_pinCommand->retract));
}