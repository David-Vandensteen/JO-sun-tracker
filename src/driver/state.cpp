#include <Arduino.h>
#include "state.h"

State::State() : _stateType(StateType::INIT) {}

State::State(StateType stateType) : _stateType(stateType) {}

bool State::isStateInit() {
  return _stateType == StateType::INIT;
}

bool State::isStateRun() {
  return _stateType == StateType::RUN;
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

State State::setStateInit() {
  return setState(StateType::INIT);
}

State State::setStateRun() {
  return setState(StateType::RUN);
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
