#include <Arduino.h>
#include "setting.h"
#include "command.h"
#include "log.h"

Command::Command()
  : _buttonPin(nullptr) {}

Command::Command(SettingBoardPinButton *buttonPin)
  : _buttonPin(buttonPin) {}

void Command::init() {
  LOG_DEBUG("Command::init");
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
      LOG_DEBUGF("Command::isScanButtonPressed state: %d", static_cast<int>(_state));
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
  LOG_DEBUG("Command:");
  LOG_DEBUGF("- Deploy button: %s", isDeployButtonPressed() ? "pressed" : "not pressed");
  LOG_DEBUGF("- Retract button: %s", isRetractButtonPressed() ? "pressed" : "not pressed");
  LOG_DEBUGF("- Scan button: %s", isScanButtonPressed() ? "pressed" : "not pressed");
  LOG_DEBUGF("- Selected tracker ID: %d", getSelectedTrackerId());
  LOG_DEBUGF("- State: %d", static_cast<int>(_state));
}

