#include <Arduino.h>
#include "tracker_state.h"

TrackerState::TrackerState() {
  _state = TrackerStateType::IDLE;
}

TrackerState::TrackerState(TrackerStateType type) {
  _state = type;
}

bool TrackerState::isIdle() {
  return _state == TrackerStateType::IDLE;
}

bool TrackerState::isAuto() {
  return _state == TrackerStateType::AUTO;
}

bool TrackerState::isManual() {
  return _state == TrackerStateType::MANUAL;
}

void TrackerState::setIdle() {
  _state = TrackerStateType::IDLE;
}

void TrackerState::setAuto() {
  _state = TrackerStateType::AUTO;
}

void TrackerState::setManual() {
  _state = TrackerStateType::MANUAL;
}