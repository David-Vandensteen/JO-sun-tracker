#include <Arduino.h>
#include "state.h"

State::State() : _stateType(StateType::INITIALIZING) {}

State::State(StateType stateType) : _stateType(stateType) {}

bool State::isStateInitializing() {
  return _stateType == StateType::INITIALIZING;
}

bool State::isStateRunning() {
  return _stateType == StateType::RUNNING;
}

bool State::isStateStopped() {
  return _stateType == StateType::STOPPED;
}

bool State::isStateError() {
  return _stateType == StateType::ERROR;
}

bool State::isStateReset() {
  return _stateType == StateType::RESET;
}

State State::setState(StateType stateType) {
  _stateType = stateType;
  return *this;
}

State State::setStateInitializing() {
  return setState(StateType::INITIALIZING);
}

State State::setStateRunning() {
  return setState(StateType::RUNNING);
}

State State::setStateStopped() {
  return setState(StateType::STOPPED);
}

State State::setStateError() {
  return setState(StateType::ERROR);
}

State State::setStateReset() {
  return setState(StateType::RESET);
}
