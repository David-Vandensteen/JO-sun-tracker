#include <Arduino.h>
#include <ArduinoLog.h>
#include "setting.h"
#include "command.h"

Command::Command(SettingBoardPinCommand *buttonPin)
  : _buttonPin(buttonPin) {}

void Command::init() {
  Log.traceln("Command::init");
  Log.noticeln("Initializing command");
  pinMode(_buttonPin->deploy, INPUT_PULLUP);
  pinMode(_buttonPin->retract, INPUT_PULLUP);
  pinMode(_buttonPin->scan, INPUT_PULLUP);
  pinMode(_buttonPin->selectedTracker, INPUT_PULLUP);
  _state = State::Idle;
  _autoButtonState.previous = false;
  _autoButtonState.current = false;
}

bool Command::isDeployButtonPressed() {
  bool pressed = !digitalRead(_buttonPin->deploy);
  if (pressed) {
    _state = State::Deploying;
  }
  return pressed;
}

bool Command::isRetractButtonPressed() {
  bool pressed = !digitalRead(_buttonPin->retract);
  if (pressed) {
    _state = State::Retracting;
  }
  return pressed;
}

bool Command::isAutoButtonPressed() {
  if (!digitalRead(_buttonPin->scan) != _autoButtonState.current) {
    _autoButtonState.previous = _autoButtonState.current;
    _autoButtonState.current = !digitalRead(_buttonPin->scan);
    if (_autoButtonState.current) {
      Log.traceln("Command::isAutoButtonPressed state: scanning");
      _state = (_state == State::Auto) ? State::Idle : State::Auto;
    }
  }
  return _autoButtonState.current;
}

bool Command::isAuto() {
  return _state == State::Auto;
}

uint8_t Command::getSelectedTrackerId() {
  return digitalRead(_buttonPin->selectedTracker);
}

#if LOG
void Command::log() {
  Log.traceln("Command:");
  Log.traceln("- Deploy button: %s", isDeployButtonPressed() ? "pressed" : "not pressed");
  Log.traceln("- Retract button: %s", isRetractButtonPressed() ? "pressed" : "not pressed");
  Log.traceln("- Auto button: %s", isAutoButtonPressed() ? "pressed" : "not pressed");
  Log.traceln("- Selected tracker ID: %d", getSelectedTrackerId());
  Log.traceln("- State: %d", static_cast<int>(_state));
}
#endif
