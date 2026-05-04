#ifndef COMMAND_H
#define COMMAND_H
#include <Arduino.h>
#include "setting.h"

class Command {
public:
  explicit Command(SettingBoardPinButton *buttonPin);
  void init();
  bool isDeployButtonPressed();
  bool isRetractButtonPressed();
  bool isAutoButtonPressed();
  bool isAuto();
  uint8_t getSelectedTrackerId();
  void log();

private:
  enum class State {
    Idle,
    Deploying,
    Retracting,
    Auto
  };
  struct ButtonState {
    bool previous = false;
    bool current = false;
  };
  SettingBoardPinButton *_buttonPin;
  ButtonState _autoButtonState;
  State _state = State::Idle;
};

#endif