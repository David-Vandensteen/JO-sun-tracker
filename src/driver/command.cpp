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
  _autoButtonState.button.previous = false;
  _autoButtonState.button.current = false;
}

bool Command::isDeployButtonPressed() {
  bool pressed = !digitalRead(_buttonPin->deploy);
  if (pressed) {
    if (_deployButtonState.pressStart == 0) _deployButtonState.pressStart = millis();
    if (millis() - _deployButtonState.pressStart >= _deployButtonState.sustain) {
      if (!_deployButtonState.button.current) {
        _deployButtonState.button.previous = _deployButtonState.button.current;
        _deployButtonState.button.current = true;
      }
      return true;
    }
  } else {
    _deployButtonState.pressStart = 0;
    if (_deployButtonState.button.current) {
      _deployButtonState.button.previous = _deployButtonState.button.current;
      _deployButtonState.button.current = false;
    }
  }
  return false;
}

bool Command::isRetractButtonPressed() {
  bool pressed = !digitalRead(_buttonPin->retract);
  if (pressed) {
    if (_retractButtonState.pressStart == 0) _retractButtonState.pressStart = millis();
    if (millis() - _retractButtonState.pressStart >= _retractButtonState.sustain) {
      if (!_retractButtonState.button.current) {
        _retractButtonState.button.previous = _retractButtonState.button.current;
        _retractButtonState.button.current = true;
      }
      return true;
    }
  } else {
    _retractButtonState.pressStart = 0;
    if (_retractButtonState.button.current) {
      _retractButtonState.button.previous = _retractButtonState.button.current;
      _retractButtonState.button.current = false;
    }
  }
  return false;
}

bool Command::isAutoButtonPressed() {
  bool pressed = !digitalRead(_buttonPin->scan);
  if (pressed) {
    if (_autoButtonState.pressStart == 0) _autoButtonState.pressStart = millis();
    if (millis() - _autoButtonState.pressStart >= _autoButtonState.sustain) {
      if (!_autoButtonState.button.current) {
        _autoButtonState.button.previous = _autoButtonState.button.current;
        _autoButtonState.button.current = true;
        Log.traceln("Command::isAutoButtonPressed state: scanning");
      }
      return true;
    }
  } else {
    _autoButtonState.pressStart = 0;
    if (_autoButtonState.button.current) {
      _autoButtonState.button.previous = _autoButtonState.button.current;
      _autoButtonState.button.current = false;
    }
  }
  return false;
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
}
#endif
