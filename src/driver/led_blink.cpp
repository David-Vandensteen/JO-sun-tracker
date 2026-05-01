#include <Arduino.h>
#include "led_blink.h"

LedBlink::LedBlink() : _pin(0) {
  _state.current = LOW;
  _state.interval = 0;
  _state.iteration = 0;
  _state.lastChange = 0;
}

LedBlink::LedBlink(uint8_t pin) : _pin(pin) {
  LedBlink::LedBlink();
  pinMode(_pin, OUTPUT);
}

LedBlink::LedBlink(uint8_t pin, unsigned long interval, uint8_t iteration) : _pin(pin) {
  pinMode(_pin, OUTPUT);
  _state.current = LOW;
  _state.interval = interval;
  _state.iteration = iteration;
  _state.lastChange = 0;
}

void LedBlink::update() {
  if (_state.iteration > 0) {
    unsigned long now = millis();
    if (now - _state.lastChange >= _state.interval) {
      _state.current = !_state.current;
      digitalWrite(_pin, _state.current);
      _state.lastChange = now;
      _state.iteration--;
    }
  }
}