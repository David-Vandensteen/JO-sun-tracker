#ifndef DRIVER_COMMAND_STATE_H
#define DRIVER_COMMAND_STATE_H
#include <Arduino.h>

enum class DriverCommandStateType {
  IDLE,
  RESET,
  STOP,
};

class DriverCommandState {
  public:
    explicit DriverCommandState();
    explicit DriverCommandState(DriverCommandStateType type);
    bool isIdle();
    bool isStop();
    bool isReset();
    void setIdle();
    void setStop();
    void setReset();

  private:
    DriverCommandStateType _state;
};
#endif