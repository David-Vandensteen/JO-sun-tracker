#ifndef COMMAND_H
#define COMMAND_H
#include <Arduino.h>
#include "setting.h"

class Command {
public:
  explicit Command();
  explicit Command(SettingBoardPinButton *buttonPin);
  void init();
  bool isDeployButtonPressed();
  bool isRetractButtonPressed();
  bool isScanButtonPressed();
  bool isScanning();
  uint8_t getSelectedTrackerId();
  void log();

private:
  enum class State {
    Idle,
    Deploying,
    Retracting,
    Scanning
  };
  struct ButtonState {
    bool previous = false;
    bool current = false;
  };
  SettingBoardPinButton *_buttonPin;
  ButtonState _scanButtonState;
  State _state = State::Idle;
};

#endif