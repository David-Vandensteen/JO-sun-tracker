#ifndef STATE_H
#define STATE_H
#include <Arduino.h>

enum class StateType {
  INIT,
  RUN,
  STOPPED,
  RESET,
  ERROR
};

class State {
public:
  explicit State();
  explicit State(StateType stateType);
  bool isStateInit();
  bool isStateRun();
  bool isStateStopped();
  bool isStateError();
  bool isStateReset();
  State setState(StateType stateType);
  State setStateInit();
  State setStateRun();
  State setStateStopped();
  State setStateError();
  State setStateReset();

private:
  StateType _stateType = StateType::INIT;
};
#endif