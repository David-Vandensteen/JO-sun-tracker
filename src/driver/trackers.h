#ifndef TRACKERS_H
#define TRACKERS_H
#include "setting.h"
#include "tracker.h"
#include "command.h"
#include "led_protocol.h"

class Trackers {
public:
  explicit Trackers(Setting *setting);
  void init();
  void update();

private:
  Setting *_setting;
  Tracker *_trackers[TRACKER_MAX];
  Command *_command;
  LedProtocol *_ledProtocol;
};

#endif