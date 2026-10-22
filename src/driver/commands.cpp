#include <Arduino.h>
#include <ArduinoLog.h>
#include <dv_dual_hold_state.h>
#include "commands.h"
#include "setting.h"
#include "command_state.h"

void Commands::onStopHold(void *ctx) {
  if (ctx == nullptr) {
    return;
  }
  Commands *self = static_cast<Commands *>(ctx);
  Log.noticeln("Command::stop pressed");
  self->_state = self->_state.setStateStop();
  // TODO: Implement stop command behavior
}

void Commands::onResetHold(void *ctx) {
  if (ctx == nullptr) {
    return;
  }
  Commands *self = static_cast<Commands *>(ctx);
  Log.noticeln("Command::reset pressed");
  self->_state = self->_state.setStateReset();
}

Commands::Commands(SettingBoardPinTrackerCommand *command)
  : _command(command) {}

void Commands::init() {
  Log.traceln("Command::init");
  Log.noticeln("Initializing command");
  _state = _state.setStateInitializing();
  pinMode(_command->stop, INPUT_PULLUP);
  _hold.init(
    500,
    Commands::onStopHold,
    1000,
    Commands::onResetHold,
    this,
    this
  );
}

CommandState Commands::getState() {
  return _state;
}

CommandState Commands::update() {
  _hold.update(digitalRead(_command->stop) == LOW);
  return _state;
}
