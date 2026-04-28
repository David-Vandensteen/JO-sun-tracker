#ifndef TRACKERS_H
#define TRACKERS_H
#include "setting.h"
#include "tracker.h"

class Trackers {
public:
  Trackers(Setting *setting);
  void init();
  void update();

private:
  Setting *_setting;
  Tracker _trackers[TRACKER_MAX];
  void waitReady();
};

#endif