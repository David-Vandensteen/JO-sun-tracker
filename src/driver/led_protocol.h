#ifndef LED_PROTOCOL_H
#define LED_PROTOCOL_H
#include <Arduino.h>
#include "led_blink.h"

class LedProtocol {
public:
  explicit LedProtocol();
  explicit LedProtocol(uint8_t pin);
  void waitReady();
  void invalidSetting();
  void update();

private:
  enum class State {
    Idle,
    Waiting,
    Error,
  };
  LedBlink _ledBlink;
  uint8_t _pin;
  State _state = State::Idle;
};

#endif