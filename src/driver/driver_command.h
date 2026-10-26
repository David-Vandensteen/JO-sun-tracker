#ifndef DRIVER_COMMAND_H
#define DRIVER_COMMAND_H
#include <Arduino.h>
#include <OneButton.h>
#include "setting.h"
#include "driver_command_state.h"

class DriverCommand {
  public:
    explicit DriverCommand(SettingBoardPinDriverCommand *settingBoardPinDriverCommand, SettingProgramCommandStop *settingProgramCommandStop);
    void init();
    DriverCommandState update();

  private:
    SettingBoardPinDriverCommand *_settingBoardPinDriverCommand;
    SettingProgramCommandStop *_settingProgramCommandStop;
    DriverCommandState _commandState;
    OneButton *_stopButton;
};

#endif