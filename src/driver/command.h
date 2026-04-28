#ifndef COMMAND_H
#define COMMAND_H
#include <Arduino.h>
#include "setting.h"

class Command {
public:
  explicit Command();
  explicit Command(SettingBoardPinButton *buttonPin);
  void init();
  bool isDeployPressed();
  bool isRetractPressed();
  bool isScanPressed();
  uint8_t getSelectedTrackerId();
  void log();

private:
  SettingBoardPinButton *_buttonPin;
};

#endif