#include <Arduino.h>
#include <ArduinoLog.h>
#include "setting.h"
#include "command.h"

Command::Command(SettingBoardPinTrackerCommand *command)
  : _command(command) {}

void Command::init() {
  Log.traceln("Command::init");
  Log.noticeln("Initializing command");
  pinMode(_command->stop, INPUT_PULLUP);
}
