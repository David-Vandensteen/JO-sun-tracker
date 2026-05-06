#ifndef DRIVER_H
#define DRIVER_H
#include <Arduino.h>
#include "setting.h"
#include "led_protocol.h"
#include "command.h"
#include "trackers.h"

class Driver {
public:
  static void init(Setting *setting);
  static void update();

private:
  static Setting *_setting;
  static LedProtocol *_ledProtocol;
  static Command *_command;
  static Trackers *_trackers;
  static void watchDog();
};

#endif