#ifndef TRACKERS_H
#define TRACKERS_H
#include "setting.h"
#include "tracker.h"

// Macros de log personnalisées
#define LOG_ERROR(msg)   do { Serial.print("[ERROR] "); Serial.println(msg); } while(0)
#define LOG_WARN(msg)    do { Serial.print("[WARN]  "); Serial.println(msg); } while(0)
#define LOG_INFO(msg)    do { Serial.print("[INFO]  "); Serial.println(msg); } while(0)
#define LOG_DEBUG(msg)   do { Serial.print("[DEBUG] "); Serial.println(msg); } while(0)

class Trackers {
public:
  explicit Trackers(Setting *setting);
  void init();
  void update();

private:
  Setting *_setting;
  Tracker _trackers[TRACKER_MAX];
  void waitReady();
};

#endif