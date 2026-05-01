#include <Arduino.h>
#include <ArduinoLog.h>
#include "setting.h"
#include "led_blink.h"
#include "led_protocol.h"


LedProtocol::LedProtocol() : _pin(0) {
  _state = State::Idle;
  _ledBlink = LedBlink(_pin);
}

LedProtocol::LedProtocol(uint8_t pin)
  : _pin(pin) {
    Log.trace("LedProtocol::LedProtocol\n");
    _state = State::Idle;
    _ledBlink = LedBlink(_pin);
  }

void LedProtocol::invalidSetting() {
  Log.fatal("Invalid setting");
  _state = State::Error;
  _ledBlink = LedBlink(_pin, 500, 10);
  while (true) {
    LedProtocol::update();
  }
}

void LedProtocol::waitReady() {
  _state = State::Waiting;
  _ledBlink = LedBlink(_pin, 1000, 5);
}

void LedProtocol::update() {
  _ledBlink.update();
}