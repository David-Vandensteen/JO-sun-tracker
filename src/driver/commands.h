#ifndef COMMANDS_H
#define COMMANDS_H
#include <Arduino.h>
#include <dv_dual_hold_state.h>
#include "setting.h"
#include "command_state.h"

class Commands {
public:
  explicit Commands(SettingBoardPinDriverCommand  *command);
  void init();
  CommandState getState();
  CommandState update();

private:
  DV_DualHoldState _hold;
  CommandState _state;
  SettingBoardPinDriverCommand *_command;
  static void onStopHold(void *ctx);
  static void onResetHold(void *ctx);
};

#endif