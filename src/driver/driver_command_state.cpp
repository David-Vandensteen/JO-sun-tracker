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

void DriverCommandState::setReset() {
  _state = DriverCommandStateType::RESET;
}

void DriverCommandState::setIdle() {
  _state = DriverCommandStateType::IDLE;
}

void DriverCommandState::setStop() {
  _state = DriverCommandStateType::STOP;
}