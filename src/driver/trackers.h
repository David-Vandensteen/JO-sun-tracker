#ifndef TRACKERS_H
#define TRACKERS_H
#include "settings.h"
#include "tracker.h"

class Trackers {
public:
  Trackers(Settings *settings);
  void init();
  bool isAutoMode();
  void setAutoMode(bool autoMode);
  void update();

private:
  bool _isAutoMode = true;
  Settings *_settings;
  Tracker _trackers[TRACKER_MAX];
  void waitReady();
};

#endif