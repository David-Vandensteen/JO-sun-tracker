#ifndef TRACKER_COMMAND_STATE_H
#define TRACKER_COMMAND_STATE_H
#include <Arduino.h>

enum class TrackerCommandStateType {
  IDLE,
  DEPLOY,
  RETRACT,
  AUTO,
  RESET,
};

class TrackerCommandState {
  public:
    explicit TrackerCommandState();
    explicit TrackerCommandState(TrackerCommandStateType type);
    bool isIdle();
    bool isDeploy();
    bool isRetract();
    bool isAuto();
    bool isReset();
    void setIdle();
    void setDeploy();
    void setRetract();
    void setAuto();
    void setReset();

  private:
    TrackerCommandStateType _state;
};
#endif
