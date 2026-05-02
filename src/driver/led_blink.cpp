#include <Arduino.h>
#include "led_blink.h"

LedBlink::LedBlink() : _pin(0), _state{LOW, 100, LED_BLINK_INFINITE, 0} {}

LedBlink::LedBlink(uint8_t pin) : _pin(pin), _state{LOW, 100, LED_BLINK_INFINITE, 0} {
  pinMode(_pin, OUTPUT);
}

LedBlink::LedBlink(uint8_t pin, unsigned long interval) : _pin(pin), _state{LOW, interval, LED_BLINK_INFINITE, 0} {
  pinMode(_pin, OUTPUT);
}

LedBlink::LedBlink(uint8_t pin, unsigned long interval, uint8_t cycle) : _pin(pin), _state{LOW, interval, cycle, 0} {
  pinMode(_pin, OUTPUT);
}

void LedBlink::update() {
  if (_state.cycle > 0 || _state.cycle == 255) {
    unsigned long now = millis();
    if (now - _state.lastChange >= _state.interval) {
      _state.current = !_state.current;
      digitalWrite(_pin, _state.current);
      _state.lastChange = now;
      if (_state.current == LOW && _state.cycle != 255) _state.cycle--;
    }
  }
}