#ifndef DRIVER_H
#define DRIVER_H
#include <Arduino.h>
#include "setting.h"
#include "led_protocol.h"
#include "command.h"
#include "trackers.h"
#include "state.h"

class Driver {
public:
  static void init(Setting *setting);
  static void update();

private:
  static Setting *_setting;
  static LedProtocol *_ledProtocol;
  static Trackers *_trackers;
  static void heartBeat();
};

#endif