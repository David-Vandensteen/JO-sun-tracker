/*
  David Vandensteen
  MIT License
  JO Sun Tracker - Driver code for Arduino Uno/ESP32 WROOM 32S
  @2026
*/

#include <Arduino.h>
#include <ArduinoLog.h>
#include "setting.h"
#include "trackers.h"

static Setting setting;
static Trackers trackers(&setting);

static void serial() {
  if (LOG) {
    Serial.begin(setting.board.serial.baudRate);
    Log.begin(LOG_LEVEL, &Serial);
    delay(2000);
  }
}

static void log() {
  Log.trace("setup\n");
  Log.notice("Starting program\n");
  Log.notice("JO Sun Tracker - version %s\n", setting.program.version);
}

void setup() {
  settingInit(&setting);
  serial();
  log();
  trackers.init();
}

void loop() {
  unsigned long currentTime = millis();
  static unsigned long lastTime = 0;
  if (currentTime - lastTime >= 30000) {
    lastTime = currentTime;
    Log.trace("WatchDog...\n");
  }
  trackers.update();
}
