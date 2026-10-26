#include <Arduino.h>
#include "driver_state.h"

// Public
DriverState::DriverState() {
  _state = DriverStateType::IDLE;
}

DriverState::DriverState(DriverStateType type) {
  _state = type;
}

bool DriverState::isIdle() {
  return _state == DriverStateType::IDLE;
}

bool DriverState::isReset() {
  return _state == DriverStateType::RESET;
}

bool DriverState::isStop() {
  return _state == DriverStateType::STOP;
}

void DriverState::setIdle() {
  _state = DriverStateType::IDLE;
}

void DriverState::setReset() {
  _state = DriverStateType::RESET;
}

void DriverState::setStop() {
  _state = DriverStateType::STOP;
}