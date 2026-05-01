#include <Arduino.h>
#include "setting.h"
#include "led_protocol.h"
#include <ArduinoLog.h>

LedProtocol::LedProtocol() : _pin(0) {}

LedProtocol::LedProtocol(uint8_t ledStatusPin)
  : _pin(ledStatusPin) {}

void LedProtocol::init() {
  Log.trace("LedProtocol::init\n");
  _state = State::Idle;
  pinMode(_pin, OUTPUT);
  _ledState.current = LOW;
  _ledState.iteration = 0;
  _ledState.interval = 0;
  _ledState.lastChange = 0;
  digitalWrite(_pin, _ledState.current);
}

void LedProtocol::invalidSetting() {
  Log.fatal("Invalid setting");
  _state = State::Error;
  while (true) {
    _ledState.current = HIGH;
    _ledState.interval = 100;
    _ledState.lastChange = millis();
    #if defined(BOARD_UNO) || defined(BOARD_NANO)
      digitalWrite(LED_BUILTIN, _ledState.current);
    #endif
    digitalWrite(_pin, _ledState.current);
    delay(_ledState.interval);
    _ledState.current = LOW;
    _ledState.lastChange = millis();
    #if defined(BOARD_UNO) || defined(BOARD_NANO)
      digitalWrite(LED_BUILTIN, _ledState.current);
    #endif
    digitalWrite(_pin, _ledState.current);
    _ledState.lastChange = millis();
    delay(_ledState.interval);
  }
}

void LedProtocol::waitReady() {
  _state = State::Waiting;
  _ledState.current = LOW;
  _ledState.interval = 1000;
  _ledState.lastChange = millis();
  _ledState.iteration = 5;
}

void LedProtocol::update() {
  if (_ledState.iteration > 0) {
    if (_state == State::Error) {
      return;
    }
    if (_state == State::Waiting) {
      Log.trace("LedProtocol::update waiting, iteration: %d\n", _ledState.iteration);
      unsigned long now = millis();
      if (now - _ledState.lastChange >= _ledState.interval) {
        _ledState.current = !_ledState.current;
        _ledState.lastChange = now;
        #if defined(BOARD_UNO) || defined(BOARD_NANO)
          digitalWrite(LED_BUILTIN, _ledState.current);
        #endif
        digitalWrite(_pin, _ledState.current);
        _ledState.iteration--;
      }
    }
  }
}