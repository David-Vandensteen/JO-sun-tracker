#include <Arduino.h>
#include "setting.h"
#include "led_protocol.h"
#include <ArduinoLog.h>

LedProtocol::LedProtocol() : _ledStatusPin(0) {}

LedProtocol::LedProtocol(uint8_t ledStatusPin)
  : _ledStatusPin(ledStatusPin) {}

void LedProtocol::init() {
  Log.trace("LedProtocol::init\n");
  pinMode(_ledStatusPin, OUTPUT);
}

void LedProtocol::invalidSetting() {
  Log.fatal("Invalid setting");
  while (true) {
    #if defined(BOARD_UNO) || defined(BOARD_NANO)
      digitalWrite(LED_BUILTIN, HIGH);
    #endif
    digitalWrite(_ledStatusPin, HIGH);
    delay(100);
    digitalWrite(_ledStatusPin, LOW);
    #if defined(BOARD_UNO) || defined(BOARD_NANO)
      digitalWrite(LED_BUILTIN, LOW);
    #endif
    delay(100);
  }
}

void LedProtocol::waitReady() {
  Log.trace("LedProtocol::waitReady\n");
  for (uint8_t i = 0; i < 3; i++) {
    #if defined(BOARD_UNO) || defined(BOARD_NANO)
      digitalWrite(LED_BUILTIN, HIGH);
    #endif
    digitalWrite(_ledStatusPin, HIGH);
    delay(1000);
    digitalWrite(_ledStatusPin, LOW);
    #if defined(BOARD_UNO) || defined(BOARD_NANO)
      digitalWrite(LED_BUILTIN, LOW);
    #endif
    delay(1000);
  }
}

void LedProtocol::update() {
  // TODO implement LED protocol to indicate status
}