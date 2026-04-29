/*
  David Vandensteen
  MIT License
  JO Sun Tracker - Driver code for Arduino Uno/ESP32 WROOM 32S
  @2026
*/

#include <Arduino.h>
#include "setting.h"
#include "trackers.h"
#include "log.h"

static Setting setting;
static Trackers trackers(&setting);

void setup() {
  settingInit(&setting);
  if (LOG_SERIAL) Serial.begin(setting.board.serial.baudRate);
  delay(2000);
  LOG_DEBUG("setup");
  trackers.init();
}
void loop() { trackers.update(); }
