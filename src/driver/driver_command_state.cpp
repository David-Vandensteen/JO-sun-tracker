#include <Arduino.h>
#include "driver_command_state.h"

// Public
DriverCommandState::DriverCommandState() {
  _state = DriverCommandStateType::IDLE;
}

DriverCommandState::DriverCommandState(DriverCommandStateType type) {
  _state = type;
}

bool DriverCommandState::isIdle() {
  return _state == DriverCommandStateType::IDLE;
}

bool DriverCommandState::isStop() {
  return _state == DriverCommandStateType::STOP;
}

bool DriverCommandState::isReset() {
  return _state == DriverCommandStateType::RESET;
}

DriverCommandStateType DriverCommandState::setReset() {
  _state = DriverCommandStateType::RESET;
  return _state;
}

DriverCommandStateType DriverCommandState::setIdle() {
  _state = DriverCommandStateType::IDLE;
  return _state;
}

DriverCommandStateType DriverCommandState::setStop() {
  _state = DriverCommandStateType::STOP;
  return _state;
}