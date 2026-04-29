#include <Arduino.h>
#include "setting.h"
#include "command.h"
#include <ArduinoLog.h>

Command::Command()
  : _buttonPin(nullptr) {}

Command::Command(SettingBoardPinButton *buttonPin)
  : _buttonPin(buttonPin) {}

void Command::init() {
  Log.trace("Command::init\n");
  pinMode(_buttonPin->deploy, INPUT_PULLUP);
  pinMode(_buttonPin->retract, INPUT_PULLUP);
  pinMode(_buttonPin->scan, INPUT_PULLUP);
  pinMode(_buttonPin->selectedTracker, INPUT_PULLUP);
  _state = State::Idle;
  _scanButtonState.previous = false;
  _scanButtonState.current = false;
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

bool Command::isScanButtonPressed() {
  if (!digitalRead(_buttonPin->scan) != _scanButtonState.current) {
    _scanButtonState.previous = _scanButtonState.current;
    _scanButtonState.current = !digitalRead(_buttonPin->scan);
    if (_scanButtonState.current) {
      Log.trace("Command::isScanButtonPressed state: scanning\n");
      _state = (_state == State::Scanning) ? State::Idle : State::Scanning;
    }
  }
  return _scanButtonState.current;
}

bool Command::isScanning() {
  return _state == State::Scanning;
}

uint8_t Command::getSelectedTrackerId() {
  return digitalRead(_buttonPin->selectedTracker);
}

void Command::log() {
  Log.trace("Command:"); // TODO
  // LOG_TRACEF("- Deploy button: %s", isDeployButtonPressed() ? "pressed" : "not pressed");
  // LOG_TRACEF("- Retract button: %s", isRetractButtonPressed() ? "pressed" : "not pressed");
  // LOG_TRACEF("- Scan button: %s", isScanButtonPressed() ? "pressed" : "not pressed");
  // LOG_TRACEF("- Selected tracker ID: %d", getSelectedTrackerId());
  // LOG_TRACEF("- State: %d", static_cast<int>(_state));
}

