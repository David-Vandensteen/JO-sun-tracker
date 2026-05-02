#ifndef LED_PROTOCOL_H
#define LED_PROTOCOL_H
#include <Arduino.h>
#include "setting.h"
#include "dv_led_blink.h"

class LedProtocol {
public:
  explicit LedProtocol();
  explicit LedProtocol(uint8_t pin);
  void waiting();
  void fatalError();
  void update();

private:
  enum class State {
    Idle,
    Waiting,
    Error,
  };
  DV_LedBlink _led;
  #if defined(BOARD_UNO) || defined(BOARD_NANO)
    DV_LedBlink _ledBuiltin = DV_LedBlink(LED_BUILTIN);
  #endif
  uint8_t _pin;
  State _state = State::Idle;
  void blink(unsigned long interval, uint8_t iteration);
};

#endif