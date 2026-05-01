#ifndef LED_PROTOCOL_H
#define LED_PROTOCOL_H

class LedProtocol {
public:
  explicit LedProtocol();
  explicit LedProtocol(uint8_t ledPin);
  void init();
  void waitReady();
  void invalidSetting();
  void update();

private:
  struct LedState {
    bool current = LOW;
    uint8_t iteration = 0;
    unsigned long interval = 0;
    unsigned long lastChange = 0;
  };
  enum class State {
    Idle,
    Waiting,
    Error,
  };
  uint8_t _pin;
  LedState _ledState;
  State _state = State::Idle;
};

#endif