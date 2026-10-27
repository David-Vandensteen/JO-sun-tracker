#include <Arduino.h>
#include "tracker_command_state.h"

// Public
TrackerCommandState::TrackerCommandState() {
  _state = TrackerCommandStateType::IDLE;
}

TrackerCommandState::TrackerCommandState(TrackerCommandStateType type) {
  _state = type;
}

bool TrackerCommandState::isIdle() {
  return _state == TrackerCommandStateType::IDLE;
}

bool TrackerCommandState::isDeploy() {
  return _state == TrackerCommandStateType::DEPLOY;
}

bool TrackerCommandState::isRetract() {
  return _state == TrackerCommandStateType::RETRACT;
}

bool TrackerCommandState::isAuto() {
  return _state == TrackerCommandStateType::AUTO;
}

void TrackerCommandState::setIdle() {
  _state = TrackerCommandStateType::IDLE;
}

void TrackerCommandState::setDeploy() {
  _state = TrackerCommandStateType::DEPLOY;
}

void TrackerCommandState::setRetract() {
  _state = TrackerCommandStateType::RETRACT;
}

void TrackerCommandState::setAuto() {
  _state = TrackerCommandStateType::AUTO;
}
