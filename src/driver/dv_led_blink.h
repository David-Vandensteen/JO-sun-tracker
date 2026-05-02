#ifndef DV_LED_BLINK_H
#define DV_LED_BLINK_H
#include <Arduino.h>

#define DV_LED_BLINK_INFINITE 255

class DV_LedBlink {
public:
  explicit DV_LedBlink();
  explicit DV_LedBlink(uint8_t pin);
  explicit DV_LedBlink(uint8_t pin, unsigned long interval);
  explicit DV_LedBlink(uint8_t pin, unsigned long interval, uint8_t cycle = DV_LED_BLINK_INFINITE);
  void update();

private:
  struct State {
    bool current = LOW;
    uint8_t cycle = DV_LED_BLINK_INFINITE;
    unsigned long interval = 100;
    unsigned long lastChange = 0;
    State(bool current, unsigned long interval, uint8_t cycle, unsigned long lastChange)
      : current(current), cycle(cycle), interval(interval), lastChange(lastChange) {}
  };
  uint8_t _pin;
  State _state;
};

#endif

