#ifndef DRIVER_H
#define DRIVER_H
#include <Arduino.h>
#include "setting.h"
#include "led_protocol.h"
#include "tracker.h"
#include "driver_state.h"
#include "driver_command.h"
#include "app_status.h"

class Driver {
public:
  static void init(Setting *setting);
  static void update();
  static void stop();
  static void reset();
  static DriverState state();

private:
  static Setting *_setting;
  static LedProtocol *_ledProtocol;
  static Tracker *_tracker;
  static DriverState _state;
  static DriverCommand *_command;
  static void heartBeat();
  static void publishStatus();
};

#endif