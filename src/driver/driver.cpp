#include <Arduino.h>
#include <ArduinoLog.h>
#include "setting.h"
#include "driver.h"

void Driver::serial(unsigned long baudRate, int logLevel) {
  Serial.begin(baudRate);
  Log.begin(logLevel, &Serial);
  delay(2000);
}

void Driver::log(char *version) {
  Log.trace("setup\n");
  Log.notice("Starting program\n");
  Log.notice("JO Sun Tracker - version %s\n", version);
}
