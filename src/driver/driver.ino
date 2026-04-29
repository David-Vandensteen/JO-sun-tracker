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

void setup() {
  settingInit(&setting);
  Serial.begin(setting.board.serial.baudRate);
  Log.begin(LOG_LEVEL, &Serial);
  delay(2000);
  Log.trace("setup\n");
  trackers.init();
}
void loop() {
  trackers.update();
}
