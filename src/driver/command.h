#ifndef COMMAND_H
#define COMMAND_H
#include <Arduino.h>
#include "setting.h"

class Command {
public:
  explicit Command(SettingBoardPinCommand *buttonPin);
  void init();
  bool isDeployButtonPressed();
  bool isRetractButtonPressed();
  bool isAutoButtonPressed();
  bool isAuto();
  uint8_t getSelectedTrackerId();
  #if LOG
  void log();
  #endif

private:
  struct ButtonState {
    bool previous = false;
    bool current = false;
  };
  struct State { // TODO: dv_hold_watcher
    ButtonState button;
    unsigned long pressStart = 0;
    unsigned long sustain = 1000;
  };
  SettingBoardPinCommand *_buttonPin;
  State _deployButtonState;
  State _retractButtonState;
  State _autoButtonState;
};

#endif