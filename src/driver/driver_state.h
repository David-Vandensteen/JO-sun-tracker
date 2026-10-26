#ifndef DRIVER_STATE_H
#define DRIVER_STATE_H
#include <Arduino.h>

enum class DriverStateType {
  IDLE,
  RESET,
  STOP
};

class DriverState {
  public:
    explicit DriverState();
    explicit DriverState(DriverStateType type);
    bool isIdle();
    bool isReset();
    bool isStop();
    void setIdle();
    void setReset();
    void setStop();

  private:
    DriverStateType _state;
};

#endif