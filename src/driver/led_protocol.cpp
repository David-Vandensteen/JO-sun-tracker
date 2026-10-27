#include <Arduino.h>
#include <ArduinoLog.h>
#include <dv_led_blink.h>
#include "setting.h"
#include "led_protocol.h"

LedProtocol::LedProtocol(uint8_t pin)
  : _pin(pin) {
    _state = State::Idle;
    _led = DV_LedBlink(_pin);
  }

void LedProtocol::blink(unsigned long interval, uint8_t iteration) {
  _led = DV_LedBlink(_pin, interval, iteration);
}

void LedProtocol::fatalError() {
  _state = State::Error;
  LedProtocol::blink(250, DV_LedBlink::FOREVER);
  while (true) {
    LedProtocol::update();
  }
}

void LedProtocol::waiting() {
  _state = State::Waiting;
  LedProtocol::blink(1000, 15);
}

void LedProtocol::update() {
  _led.update();
}