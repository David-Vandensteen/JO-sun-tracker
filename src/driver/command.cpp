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
}

bool Command::isDeployPressed() {
  return !digitalRead(_buttonPin->deploy);
}

bool Command::isRetractPressed() {
  return !digitalRead(_buttonPin->retract);
}

bool Command::isScanPressed() {
  return !digitalRead(_buttonPin->scan);
}

uint8_t Command::getSelectedTrackerId() {
  return digitalRead(_buttonPin->selectedTracker);
}

void Command::log() {
  LOG_DEBUG("Command:");
  LOG_DEBUG("- Deploy button: " + String(isDeployPressed() ? "pressed" : "not pressed"));
  LOG_DEBUG("- Retract button: " + String(isRetractPressed() ? "pressed" : "not pressed"));
  LOG_DEBUG("- Scan button: " + String(isScanPressed() ? "pressed" : "not pressed"));
  LOG_DEBUG("- Selected tracker ID: " + String(getSelectedTrackerId()));
}