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
  Log.trace("Driver::log\n");
  Log.notice("Starting program\n");
  Log.notice("JO Sun Tracker - version %s\n", version);
}

void Driver::watchDog() {
  unsigned long currentTime = millis();
  static unsigned long lastTime = 0;
  if (currentTime - lastTime >= 30000) {
    lastTime = currentTime;
    Log.notice("WatchDog\n");
  }
}
