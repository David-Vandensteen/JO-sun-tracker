#include <Arduino.h>
#include <ArduinoLog.h>
#include "setting.h"
#include "driver.h"
#include "led_protocol.h"

void Driver::serial(unsigned long baudRate, int logLevel) {
  Serial.begin(baudRate);
  Log.begin(logLevel, &Serial);
  delay(2000);
}

LedProtocol Driver::createLedProtocol(uint8_t pin) {
  return LedProtocol(pin);
}

void Driver::log(char *version) {
  Log.trace("Driver::log\n");
  Log.notice("Starting program\n");
  Log.notice("JO Sun Tracker - version %s\n", version);
}

void Driver::watchDog() {
  #if defined(WATCHDOG_INTERVAL)
  if (LOG && WATCHDOG) {
    unsigned long currentTime = millis();
    static unsigned long lastTime = 0;
    if (currentTime - lastTime >= WATCHDOG_INTERVAL) {
      lastTime = currentTime;
      Serial.println("Watchdog");
    }
  }
  #endif
}
