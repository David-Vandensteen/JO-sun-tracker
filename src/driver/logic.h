#ifndef LOGIC_H
#define LOGIC_H
#include "settings.h"
#include "ldr.h"
#include "motor.h"

class Logic {
public:
  Logic(Settings *settings);
  void init();
  void runAuto();
  void runManual();
  void deploy();
  void retract();
  void scan();
private:
  Settings* _settings;
  LDRs* _ldrs;
  Motors* _motors;
};

#endif