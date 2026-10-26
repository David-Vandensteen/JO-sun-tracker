#include <Arduino.h>
#include <OneButton.h>
#include "driver_command.h"
#include "driver_command_state.h"

#define DEBOUNCE_THRESHOLD 1000

DriverCommand::DriverCommand(SettingBoardPinDriverCommand *pinCommand) {
  _settingBoardPinDriverCommand = pinCommand;
  _commandState = DriverCommandState();
}

void DriverCommand::init() {
  pinMode(_settingBoardPinDriverCommand->stop, INPUT_PULLUP);
  _stopButton = new OneButton(_settingBoardPinDriverCommand->stop, true, true);
  _stopButton->setPressMs(DEBOUNCE_THRESHOLD);
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
