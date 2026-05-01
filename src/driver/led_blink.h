#ifndef LED_BLINK_H
#define LED_BLINK_H
#include <Arduino.h>

class LedBlink {
public:
  explicit LedBlink();
  explicit LedBlink(uint8_t pin);
  explicit LedBlink(uint8_t pin, unsigned long interval, uint8_t iteration);
  void update();

private:
  struct State {
    bool current = LOW;
    uint8_t iteration = 0;
    unsigned long interval = 0;
    unsigned long lastChange = 0;
  };
  uint8_t _pin;
  State _state;
};

#endif

