#ifndef DRIVER_STATE_H
#define DRIVER_STATE_H
#include <Arduino.h>

enum class DriverStateType {
  IDLE,
  RESET
};

class DriverState {
  public:
    explicit DriverState();
    explicit DriverState(DriverStateType type);
    bool isIdle();
    bool isReset();
    void setIdle();
    void setReset();

  private:
    DriverStateType _state;
};

#endif