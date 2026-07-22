#include <Arduino.h>
#include <ArduinoLog.h>
#include "setting.h"
#include "command.h"

Command::Command(SettingBoardPinTrackerCommand *buttonPin)
  : _buttonPin(buttonPin) {}

void Command::init() {
  Log.traceln("Command::init");
  Log.noticeln("Initializing command");
  pinMode(_buttonPin->stop, INPUT_PULLUP);
}
