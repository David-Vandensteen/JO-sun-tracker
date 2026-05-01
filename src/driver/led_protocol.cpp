#include <Arduino.h>
#include <ArduinoLog.h>
#include "setting.h"
#include "led_blink.h"
#include "led_protocol.h"

LedProtocol::LedProtocol() : _pin(0) {
  Log.trace("LedProtocol::LedProtocol\n");
  _state = State::Idle;
  _led = LedBlink(_pin);
  #if defined(BOARD_UNO) || defined(BOARD_NANO)
    _ledBuiltin = LedBlink(LED_BUILTIN);
  #endif
}

LedProtocol::LedProtocol(uint8_t pin)
  : _pin(pin) {
    Log.trace("LedProtocol::LedProtocol\n");
    LedProtocol::LedProtocol();
    _state = State::Idle;
    _led = LedBlink(_pin);
  }

void LedProtocol::blink(unsigned long interval, uint8_t iteration) {
  _led = LedBlink(_pin, interval, iteration);
  #if defined(BOARD_UNO) || defined(BOARD_NANO)
    _ledBuiltin = LedBlink(LED_BUILTIN, interval, iteration);
  #endif
}

void LedProtocol::fatalError() {
  Log.fatal("Invalid setting");
  _state = State::Error;
  LedProtocol::blink(500, 10);
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
  #if defined(BOARD_UNO) || defined(BOARD_NANO)
    _ledBuiltin.update();
  #endif
}