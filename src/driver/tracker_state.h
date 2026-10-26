#ifndef TRACKER_STATE_H
#define TRACKER_STATE_H
#include <Arduino.h>

enum TrackerStateType {
  IDLE,
  AUTO,
  MANUAL
};

class TrackerState {
  public:
    explicit TrackerState();
    explicit TrackerState(TrackerStateType type);
    bool isIdle();
    bool isAuto();
    bool isManual();
    void setIdle();
    void setAuto();
    void setManual();

  private:
    TrackerStateType _state;
};

#endif // TRACKER_STATE_H