#ifndef STATE_H
#define STATE_H
#include <Arduino.h>

enum class StateType {
  INITIALIZING,
  RUNNING,
  STOPPED,
  RESET,
  ERROR
};

class State {
public:
  explicit State();
  explicit State(StateType stateType);
  bool isStateInitializing();
  bool isStateRunning();
  bool isStateStopped();
  bool isStateError();
  bool isStateReset();
  State setState(StateType stateType);
  State setStateInitializing();
  State setStateRunning();
  State setStateStopped();
  State setStateError();
  State setStateReset();

private:
  StateType _stateType = StateType::INITIALIZING;
};
#endif