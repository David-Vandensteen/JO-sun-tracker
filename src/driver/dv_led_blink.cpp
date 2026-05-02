#include <Arduino.h>
#include "dv_led_blink.h"

DV_LedBlink::DV_LedBlink() : _pin(0), _state{LOW, 100, DV_LED_BLINK_INFINITE, 0} {}

DV_LedBlink::DV_LedBlink(uint8_t pin) : _pin(pin), _state{LOW, 100, DV_LED_BLINK_INFINITE, 0} {
  pinMode(_pin, OUTPUT);
}

DV_LedBlink::DV_LedBlink(uint8_t pin, unsigned long interval) : _pin(pin), _state{LOW, interval, DV_LED_BLINK_INFINITE, 0} {
  pinMode(_pin, OUTPUT);
}

DV_LedBlink::DV_LedBlink(uint8_t pin, unsigned long interval, uint8_t cycle) : _pin(pin), _state{LOW, interval, cycle, 0} {
  pinMode(_pin, OUTPUT);
}

void DV_LedBlink::update() {
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