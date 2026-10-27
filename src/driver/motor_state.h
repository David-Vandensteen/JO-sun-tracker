#ifndef MOTOR_STATE_H
#define MOTOR_STATE_H
#include <Arduino.h>

enum class MotorStateType {
  IDLE,
  DEPLOY,
  RETRACT
};

class MotorState {
  public:
    explicit MotorState();
    explicit MotorState(MotorStateType type);
    bool isIdle();
    bool isDeploy();
    bool isRetract();
    void setIdle();
    void setDeploy();
    void setRetract();

  private:
    MotorStateType _state;
};

#endif // MOTOR_STATE_H
