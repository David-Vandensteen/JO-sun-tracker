#include <Arduino.h>
#include "motor_state.h"

// Public
MotorState::MotorState() {
  _state = MotorStateType::IDLE;
}

MotorState::MotorState(MotorStateType type) {
  _state = type;
}

bool MotorState::isIdle() {
  return _state == MotorStateType::IDLE;
}

bool MotorState::isDeploy() {
  return _state == MotorStateType::DEPLOY;
}

bool MotorState::isRetract() {
  return _state == MotorStateType::RETRACT;
}

void MotorState::setIdle() {
  _state = MotorStateType::IDLE;
}

void MotorState::setDeploy() {
  _state = MotorStateType::DEPLOY;
}

void MotorState::setRetract() {
  _state = MotorStateType::RETRACT;
}
