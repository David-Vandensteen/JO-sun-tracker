#ifndef COMMAND_H
#define COMMAND_H
#include <Arduino.h>
#include "setting.h"

class Command {
public:
  explicit Command(SettingBoardPinTrackerCommand *buttonPin);
  void init();
  #if LOG
  void log();
  #endif

private:
  SettingBoardPinTrackerCommand *_buttonPin;
  struct ButtonState {
    bool previous = false;
    bool current = false;
  };
  struct State { // TODO: dv_sustain_state
    ButtonState button;
    unsigned long pressStart = 0;
    unsigned long sustain = 1000;
  };
};

#endif

/*
class SustainBool {
public:
  SustainBool(unsigned long sustainMs = 1000) : _sustain(sustainMs) {}
  bool update(bool state) {
    if (state) {
      if (_start == 0) _start = millis();
      if (millis() - _start >= _sustain) return true;
    } else {
      _start = 0;
    }
    return false;
  }
  void setSustain(unsigned long ms) { _sustain = ms; }
private:
  unsigned long _start = 0;
  unsigned long _sustain;
};
*/