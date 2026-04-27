#ifndef TRACKERS_H
#define TRACKERS_H
#include "settings.h"
#include "tracker.h"

class Trackers {
public:
  Trackers(Settings *settings);
  void init();
  void update();

private:
  Settings *_settings;
  Tracker _trackers[TRACKER_MAX];
};

#endif