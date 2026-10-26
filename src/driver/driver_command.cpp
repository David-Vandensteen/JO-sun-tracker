#include <Arduino.h>
#include <OneButton.h>
#include "driver_command.h"
#include "driver_command_state.h"

DriverCommand::DriverCommand(SettingBoardPinDriverCommand *pinCommand, SettingProgramCommandStop *programCommandStop) {
  _settingBoardPinDriverCommand = pinCommand;
  _settingProgramCommandStop = programCommandStop;
  _commandState = DriverCommandState();
}

void DriverCommand::init() {
  pinMode(_settingBoardPinDriverCommand->stop, INPUT_PULLUP);
  _stopButton = new OneButton(_settingBoardPinDriverCommand->stop, true, true);
  _stopButton->setPressMs(_settingProgramCommandStop->pressMs);
  _stopButton->attachLongPressStart([](void *param) {
    DriverCommand *self = static_cast<DriverCommand *>(param);
    // Handle long press start event
    if (self->_commandState.isIdle()) {
      self->_commandState.setStop();
    } else if (self->_commandState.isStop()) {
      self->_commandState.setReset();
    }
  }, this);
}

DriverCommandState DriverCommand::update() {
  _stopButton->tick();
  return _commandState;
}
