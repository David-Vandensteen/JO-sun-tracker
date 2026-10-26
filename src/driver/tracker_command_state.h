#ifndef TRACKER_COMMAND_STATE_H
#define TRACKER_COMMAND_STATE_H
#include <Arduino.h>

enum class TrackerCommandStateType {
  IDLE,
  DEPLOY,
  RETRACT,
};

class TrackerCommandState {
  public:
    explicit TrackerCommandState();
    explicit TrackerCommandState(TrackerCommandStateType type);
    bool isIdle();
    bool isDeploy();
    bool isRetract();
    void setIdle();
    void setDeploy();
    void setRetract();

  private:
    TrackerCommandStateType _state;
};
#endif
